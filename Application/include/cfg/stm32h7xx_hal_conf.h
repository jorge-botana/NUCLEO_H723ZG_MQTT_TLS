// ------------------------------------------------------------------------------
// STM32H7XX_HAL_CONF
//
// - Archivo de configuración de los drivers HAL y del BSP.
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

#ifndef   STM32H7XX_HAL_CONF_H
#define   STM32H7XX_HAL_CONF_H

// -------------------------------- HAL modules ---------------------------------

#define   HAL_CORTEX_MODULE_ENABLED
#define   HAL_DMA_MODULE_ENABLED
#define   HAL_ETH_MODULE_ENABLED
#define   HAL_FLASH_MODULE_ENABLED
#define   HAL_GPIO_MODULE_ENABLED
#define   HAL_PWR_MODULE_ENABLED
#define   HAL_RCC_MODULE_ENABLED
#define   HAL_RNG_MODULE_ENABLED
#define   HAL_RTC_MODULE_ENABLED
#define   HAL_TIM_MODULE_ENABLED
#define   HAL_UART_MODULE_ENABLED

// ------------------------ Oscillator values adaptation ------------------------

#define   HSE_VALUE                                                       8000000
#define   HSE_STARTUP_TIMEOUT                                                 100
#define   CSI_VALUE                                                       4000000
#define   HSI_VALUE                                                      64000000
#define   LSE_VALUE                                                         32768
#define   LSE_STARTUP_TIMEOUT                                                5000
#define   LSI_VALUE                                                         32000
#define   EXTERNAL_CLOCK_VALUE                                           12288000

// ---------------------------- System configuration ----------------------------

#define   VDD_VALUE                                                          3300
#define   TICK_INT_PRIORITY                                                    15

// --------------------------- Ethernet configuration ---------------------------

#define   ETH_TX_DESC_CNT                                                       4
#define   ETH_RX_DESC_CNT                                                       4

#define   ETH_MAC_ADDR0                                                      0x00
#define   ETH_MAC_ADDR1                                                      0xC0
#define   ETH_MAC_ADDR2                                                      0xDF
#define   ETH_MAC_ADDR3                                                      0x24
#define   ETH_MAC_ADDR4                                                      0xBA
#define   ETH_MAC_ADDR5                                                      0x6B

// -------------------------------- HAL includes --------------------------------

#include "stm32h7xx_hal_rcc.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_dma.h"
#include "stm32h7xx_hal_eth.h"

#include "stm32h7xx_hal_cortex.h"
#include "stm32h7xx_hal_flash.h"
#include "stm32h7xx_hal_pwr.h"
#include "stm32h7xx_hal_rng.h"
#include "stm32h7xx_hal_rtc.h"
#include "stm32h7xx_hal_tim.h"
#include "stm32h7xx_hal_uart.h"

// --------------------------------- No assert ----------------------------------

#define   assert_param(expr)                                            ((void)0)

// ------------------------------------------------------------------------------

#endif // STM32H7XX_HAL_CONF_H
