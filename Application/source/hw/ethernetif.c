// ------------------------------------------------------------------------------
// ETHERNETIF
//
// - Interfaz de red para lwIP en modo multitarea con CMSIS-RTOS2, haciendo uso
//   del driver de Ethernet proporcionado por STMicroelectronics para un LAN8742.
//
// ------------------------------------------------------------------------------
// Copyright (c) 2017 STMicroelectronics.
// All rights reserved.
//
// This software component is licensed by ST under BSD 3-Clause license, the
// "License"; You may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
//
//                                           opensource.org/licenses/BSD-3-Clause
// ------------------------------------------------------------------------------


#include "ethernetif.h"

#include "cmsis_os.h"

#include "lwip/etharp.h"

#include "lan8742.h"


#define   IFNAME0                                                             's'
#define   IFNAME1                                                             't'

#define   INTERFACE_THREAD_STACK_SIZE                    configMINIMAL_STACK_SIZE
#define   TIME_WAITING_FOR_INPUT                                    osWaitForever

#define   ETH_RX_BUFFER_SIZE                                                 1524
#define   ETH_DMA_TRANSMIT_TIMEOUT                                             20


LWIP_MEMPOOL_DECLARE(RX_POOL, 10, sizeof(struct pbuf_custom), "Zero-copy RX PBUF pool");


ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT]     __attribute__((section(".RxDecripSection")));
ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT]     __attribute__((section(".TxDecripSection")));
uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_RX_BUFFER_SIZE] __attribute__((section(".RxArraySection" )));

ETH_HandleTypeDef  heth     = {0};
ETH_TxPacketConfig TxConfig = {0};

osSemaphoreId RxPktSemaphore = NULL;


static void low_level_init(struct netif *netif);
static void ethernetif_input(void *arg);
static struct pbuf *low_level_input(struct netif *netif);
static err_t low_level_output(struct netif *netif, struct pbuf *p);

static void pbuf_free_custom(struct pbuf *p);

static int32_t ETH_PHY_IO_Init(void);
static int32_t ETH_PHY_IO_DeInit(void);
static int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal);
static int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal);
static int32_t ETH_PHY_IO_GetTick(void);


err_t ethernetif_init(struct netif *netif)
{
	// Set the interface name.
	netif->name[0]    = IFNAME0;
	netif->name[1]    = IFNAME1;

	// Set the interface output functions.
	netif->output     = etharp_output;
	netif->linkoutput = low_level_output;

	// Initialize the hardware.
	low_level_init(netif);

	return ERR_OK;
}


static void low_level_init(struct netif *netif)
{
	// Initialize Ethernet peripheral.
	heth.Instance            = ETH;
	heth.Init.MACAddr[0]     = ETH_MAC_ADDR0;
	heth.Init.MACAddr[1]     = ETH_MAC_ADDR1;
	heth.Init.MACAddr[2]     = ETH_MAC_ADDR2;
	heth.Init.MACAddr[3]     = ETH_MAC_ADDR3;
	heth.Init.MACAddr[4]     = ETH_MAC_ADDR4;
	heth.Init.MACAddr[5]     = ETH_MAC_ADDR5;
	heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
	heth.Init.TxDesc         = DMATxDscrTab;
	heth.Init.RxDesc         = DMARxDscrTab;
	heth.Init.RxBuffLen      = ETH_RX_BUFFER_SIZE;
	HAL_ETH_Init(&heth);

	// Set Tx packet config common parameters.
	TxConfig.Attributes   = ETH_TX_PACKETS_FEATURES_CSUM | \
	                        ETH_TX_PACKETS_FEATURES_CRCPAD;
	TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;

	// Initialize the RX POOL.
	LWIP_MEMPOOL_INIT(RX_POOL);

	// Set the Maximum Transmission Unit (MTU).
	netif->mtu        = ETH_MAX_PAYLOAD;

	// Set MAC hardware address.
	netif->hwaddr[0]  = ETH_MAC_ADDR0;
	netif->hwaddr[1]  = ETH_MAC_ADDR1;
	netif->hwaddr[2]  = ETH_MAC_ADDR2;
	netif->hwaddr[3]  = ETH_MAC_ADDR3;
	netif->hwaddr[4]  = ETH_MAC_ADDR4;
	netif->hwaddr[5]  = ETH_MAC_ADDR5;
	netif->hwaddr_len = ETH_HWADDR_LEN;

	// Set device capabilities.
	netif->flags     |= NETIF_FLAG_BROADCAST | \
	                    NETIF_FLAG_ETHARP;

	// Assign memory buffers to DMA Rx descriptors.
	int i = 0;
	for(i = 0; i < ETH_RX_DESC_CNT; i++)
	{
		HAL_ETH_DescAssignMemory(&heth, i, Rx_Buff[i], NULL);
	}

	// Create a binary semaphore used for informing ethernetif of frame
	// reception.
	RxPktSemaphore = osSemaphoreNew(1, 1, NULL);

	// Create the task that handles the ETH_MAC.
	const osThreadAttr_t attributes =
	{
		.stack_size = INTERFACE_THREAD_STACK_SIZE,
		.priority   = osPriorityRealtime
	};
	osThreadNew(ethernetif_input, netif, &attributes);

	// Set PHY IO functions.
	lan8742_Object_t LAN8742       = {0};
	lan8742_IOCtx_t  LAN8742_IOCtx =
	{
		ETH_PHY_IO_Init,
		ETH_PHY_IO_DeInit,
		ETH_PHY_IO_WriteReg,
		ETH_PHY_IO_ReadReg,
		ETH_PHY_IO_GetTick
	};
	LAN8742_RegisterBusIO(&LAN8742, &LAN8742_IOCtx);

	// Initialize the LAN8742 ETH PHY.
	LAN8742_Init(&LAN8742);

	// Get link state.
	int32_t  PHYLinkState = LAN8742_GetLinkState(&LAN8742);
	uint32_t speed        = 0;
	uint32_t duplex       = 0;
	switch(PHYLinkState)
	{
		case LAN8742_STATUS_100MBITS_FULLDUPLEX:
			speed  = ETH_SPEED_100M;
			duplex = ETH_FULLDUPLEX_MODE;
			break;
		case LAN8742_STATUS_100MBITS_HALFDUPLEX:
			speed  = ETH_SPEED_100M;
			duplex = ETH_HALFDUPLEX_MODE;
			break;
		case LAN8742_STATUS_10MBITS_FULLDUPLEX:
			speed  = ETH_SPEED_10M;
			duplex = ETH_FULLDUPLEX_MODE;
			break;
		case LAN8742_STATUS_10MBITS_HALFDUPLEX:
			speed  = ETH_SPEED_10M;
			duplex = ETH_HALFDUPLEX_MODE;
			break;
		default:
			speed  = ETH_SPEED_100M;
			duplex = ETH_FULLDUPLEX_MODE;
			break;
	}

	// Set MAC config.
	ETH_MACConfigTypeDef MACConf = {0};
	HAL_ETH_GetMACConfig(&heth, &MACConf);
	MACConf.Speed      = speed;
	MACConf.DuplexMode = duplex;
	HAL_ETH_SetMACConfig(&heth, &MACConf);

	// Start Ethernet in interrupt mode.
	HAL_ETH_Start_IT(&heth);
}


static void ethernetif_input(void *arg)
{
	struct pbuf  *p     = NULL;
	struct netif *netif = arg;

	for( ;; )
	{
		if(osSemaphoreAcquire(RxPktSemaphore, TIME_WAITING_FOR_INPUT) == osOK)
		{
			do
			{
				p = low_level_input(netif);
				if(p != NULL)
				{
					if(netif->input(p, netif) != ERR_OK)
					{
						pbuf_free(p);
					}
				}
			}
			while(p != NULL);
		}
	}
}


static struct pbuf *low_level_input(struct netif *netif)
{
	int i = 0;
	struct pbuf *p = NULL;
	ETH_BufferTypeDef RxBuff[ETH_RX_DESC_CNT] = {0};

	for(i = 0; i < ETH_RX_DESC_CNT - 1; i++)
	{
		RxBuff[i].next = &RxBuff[i + 1];
	}

	if(HAL_ETH_GetRxDataBuffer(&heth, RxBuff) == HAL_OK)
	{
		uint32_t framelength = 0;

		HAL_ETH_GetRxDataLength(&heth, &framelength);

		// Build Rx descriptor to be ready for next data reception.
		HAL_ETH_BuildRxDescriptors(&heth);

		// Invalidate data cache for ETH Rx buffers.
		SCB_InvalidateDCache_by_Addr(RxBuff->buffer, framelength);

		struct pbuf_custom *custom_pbuf = LWIP_MEMPOOL_ALLOC(RX_POOL);
		if(custom_pbuf != NULL)
		{
			custom_pbuf->custom_free_function = pbuf_free_custom;
			p = pbuf_alloced_custom(PBUF_RAW, framelength, PBUF_REF, custom_pbuf, RxBuff->buffer, framelength);
		}
	}

	return p;
}


static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
	int i = 0;
	struct pbuf *q = NULL;
	ETH_BufferTypeDef Txbuffer[ETH_TX_DESC_CNT] = {0};

	for(q = p; q != NULL; q = q->next)
	{
		if(i >= ETH_TX_DESC_CNT)
		{
			return ERR_IF;
		}

		Txbuffer[i].buffer = q->payload;
		Txbuffer[i].len    = q->len;

		if(i > 0)
		{
			Txbuffer[i - 1].next = &Txbuffer[i];
		}

		if(q->next == NULL)
		{
			Txbuffer[i    ].next = NULL;
		}

		i++;
	}

	TxConfig.Length   = p->tot_len;
	TxConfig.TxBuffer = Txbuffer;

	HAL_ETH_Transmit(&heth, &TxConfig, ETH_DMA_TRANSMIT_TIMEOUT);

	return ERR_OK;
}


u32_t sys_now(void)
{
	return HAL_GetTick();
}


int ETH_Link_Status(void)
{
	uint32_t regvalue = 0;
	HAL_ETH_ReadPHYRegister(&heth, 0, LAN8742_BSR, &regvalue);
	if((regvalue & LAN8742_BSR_LINK_STATUS) == RESET)
	{
		return -1;
	}

	return 0;
}


void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(heth->Instance == ETH)
	{
		// Enable the Ethernet clocks.
		__HAL_RCC_ETH1MAC_CLK_ENABLE();
		__HAL_RCC_ETH1TX_CLK_ENABLE ();
		__HAL_RCC_ETH1RX_CLK_ENABLE ();

		// Enable the GPIOs clocks.
		__HAL_RCC_GPIOA_CLK_ENABLE  ();
		__HAL_RCC_GPIOB_CLK_ENABLE  ();
		__HAL_RCC_GPIOC_CLK_ENABLE  ();
		__HAL_RCC_GPIOG_CLK_ENABLE  ();

		// Ethernet pins configuration (RMII mode).
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF11_ETH;

		// - REF_CLK -> PA1
		// - MDIO    -> PA2
		// - CRS_DV  -> PA7
		GPIO_InitStruct.Pin       = GPIO_PIN_1  | \
		                            GPIO_PIN_2  | \
		                            GPIO_PIN_7 ;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		// - TXD1    -> PB13
		GPIO_InitStruct.Pin       = GPIO_PIN_13;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		// - MDC     -> PC1
		// - RXD0    -> PC4
		// - RXD1    -> PC5
		GPIO_InitStruct.Pin       = GPIO_PIN_1  | \
		                            GPIO_PIN_4  | \
		                            GPIO_PIN_5 ;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		// - RX_ER   -> PG2
		// - TX_EN   -> PG11
		// - TXD0    -> PG13
		GPIO_InitStruct.Pin       = GPIO_PIN_2  | \
		                            GPIO_PIN_11 | \
		                            GPIO_PIN_13;
		HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

		// Enable the Ethernet global interrupt.
		HAL_NVIC_SetPriority(ETH_IRQn, 7, 0);
		HAL_NVIC_EnableIRQ  (ETH_IRQn);
	}
}


void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
	osSemaphoreRelease(RxPktSemaphore);
}


static void pbuf_free_custom(struct pbuf *p)
{
	struct pbuf_custom *custom_pbuf = (struct pbuf_custom *)p;
	LWIP_MEMPOOL_FREE(RX_POOL, custom_pbuf);
}


static int32_t ETH_PHY_IO_Init(void)
{
	HAL_ETH_SetMDIOClockRange(&heth);

	return 0;
}


static int32_t ETH_PHY_IO_DeInit(void)
{
	return 0;
}


static int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal)
{
	if(HAL_ETH_ReadPHYRegister(&heth, DevAddr, RegAddr, pRegVal) != HAL_OK)
	{
		return -1;
	}

	return 0;
}


static int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal)
{
	if(HAL_ETH_WritePHYRegister(&heth, DevAddr, RegAddr, RegVal) != HAL_OK)
	{
		return -1;
	}

	return 0;
}


static int32_t ETH_PHY_IO_GetTick(void)
{
	return HAL_GetTick();
}
