// ------------------------------------------------------------------------------
// UART
//
// - Retarget de printf() a puerto serie.
//
// ------------------------------------------------------------------------------
// Copyright (c) 2021 Jorge Botana Mtz. de Ibarreta
//
// Este archivo se encuentra bajo los términos de la Licencia MIT. Debería
// haberse proporcionado una copia de ella junto a este fichero. Si no es así, se
// puede encontrar en el siguiente enlace:
//
//                                            https://opensource.org/licenses/MIT
// ------------------------------------------------------------------------------


#include "uart.h"


#define   UART_INSTANCE                                                    USART3
#define   UART_PORT                                                         GPIOD
#define   TX_PIN                                                       GPIO_PIN_8
#define   RX_PIN                                                       GPIO_PIN_9

#define   UART_CLK_EN()                             __HAL_RCC_USART3_CLK_ENABLE()
#define   GPIO_CLK_EN()                              __HAL_RCC_GPIOD_CLK_ENABLE()
#define   PERIPHERAL_CLK                                     RCC_PERIPHCLK_USART3
#define   UART_CLK                               RCC_USART234578CLKSOURCE_D2PCLK1
#define   UART_ALTERNATE                                          GPIO_AF7_USART3


UART_HandleTypeDef huart = {0};


void __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart, (uint8_t *)&ch, 1, 0xFFFF);
}


void UART_Init(void)
{
	huart.Instance        = UART_INSTANCE;
	huart.Init.BaudRate   = 115200;
	huart.Init.WordLength = UART_WORDLENGTH_8B;
	huart.Init.StopBits   = UART_STOPBITS_1;
	huart.Init.Parity     = UART_PARITY_NONE;
	huart.Init.Mode       = UART_MODE_TX_RX;
	huart.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
}


void UART_Start(void)
{
	HAL_UART_Init(&huart);
}


void UART_Stop(void)
{
	HAL_UART_DeInit(&huart);
}


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	GPIO_InitTypeDef         GPIO_InitStruct     = {0};

	if(huart->Instance == UART_INSTANCE)
	{
		PeriphClkInitStruct.PeriphClockSelection      = PERIPHERAL_CLK;
		PeriphClkInitStruct.Usart234578ClockSelection = UART_CLK;
		HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		UART_CLK_EN();

		GPIO_CLK_EN();
		GPIO_InitStruct.Pin       = TX_PIN | RX_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = UART_ALTERNATE;
		HAL_GPIO_Init(UART_PORT, &GPIO_InitStruct);
	}
}
