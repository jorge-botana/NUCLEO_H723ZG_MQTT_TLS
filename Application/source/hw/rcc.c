// ------------------------------------------------------------------------------
// RCC
//
// - Inicialización de los osciladores y relojes del sistema.
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


#include "rcc.h"


void RCC_Init(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	// Configurar la fuente de alimentación del sistema.
	HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

	// Configurar la tensión de salida del regulador interno principal.
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);
	while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

	// Configurar el LSE.
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

	// Habilitar los osciladores.
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | \
	                                   RCC_OSCILLATORTYPE_LSE | \
	                                   RCC_OSCILLATORTYPE_HSI48;

	// System oscillator:
	// -> HSE   =  8     MHz (bypass clock source)
	RCC_OscInitStruct.HSEState       = RCC_HSE_BYPASS;

	// RTC oscillator:
	// -> LSE   = 32.768 kHz (crystal/ceramic resonator)
	RCC_OscInitStruct.LSEState       = RCC_LSE_ON;

	// RNG oscillator:
	// -> HSI48 = 48     MHz
	RCC_OscInitStruct.HSI48State     = RCC_HSI48_ON;

	// PLL setup.
	RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;

	// PLL source MUX = HSE
	RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;

	// DIVM - Division factor for PLL VCO input clock.
	RCC_OscInitStruct.PLL.PLLM       =   4;

	// DIVN - Multiplication factor for PLL VCO output clock.
	RCC_OscInitStruct.PLL.PLLN       = 275;

	// DIVP - Division factor for system clock.
	RCC_OscInitStruct.PLL.PLLP       =   1;

	// DIVQ - Division factor for peripheral clocks.
	RCC_OscInitStruct.PLL.PLLQ       =   2;

	// DIVR - Division factor for peripheral clocks.
	RCC_OscInitStruct.PLL.PLLR       =   2;

	// PLL1 clock input range frequency between 2 and 4 MHz:
	// -> HSE * DIVM = 8 / 4 = 2 MHz
	RCC_OscInitStruct.PLL.PLLRGE     = RCC_PLL1VCIRANGE_1;

	// PLL1 clock output range.
	RCC_OscInitStruct.PLL.PLLVCOSEL  = RCC_PLL1VCOWIDE;

	// Fractional part of the multiplication factor.
	RCC_OscInitStruct.PLL.PLLFRACN   =   0;

	// Inicializar los osciladores.
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	// Habilitar los relojes.
	RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_SYSCLK  | \
	                                   RCC_CLOCKTYPE_HCLK    | \
	                                   RCC_CLOCKTYPE_D1PCLK1 | \
	                                   RCC_CLOCKTYPE_PCLK1   | \
	                                   RCC_CLOCKTYPE_PCLK2   | \
	                                   RCC_CLOCKTYPE_D3PCLK1;

	// System clock MUX = PLLCLK
	// -> SYSCLK = HSE *   DIVM1   * DIVN1 *   DIVP1   =
	//           =   8 * ( 1 / 4 ) *   275 * ( 1 / 1 ) = 550   MHz
	RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;

	// System clock (SYSCLK ) prescaler:
	// -> D1CPRE         = ( 1 / 1 ) ---> D1CPRE clock = 550   MHz
	RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;

	// AHB    clock (HCLK   ) prescaler:
	// -> HPRE           = ( 1 / 2 ) --->   HCLK clock = 275   MHz
	RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;

	// APB3   clock (D1PCLK1) prescaler:
	// -> D1PPRE         = ( 1 / 2 ) --->   APB3 clock = 137.5 MHz
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;

	// APB1   clock (PCLK1  ) prescaler:
	// -> D2PPRE1        = ( 1 / 2 ) --->   APB1 clock = 137.5 MHz
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;

	// APB2   clock (PCLK2  ) prescaler:
	// -> D2PPRE2        = ( 1 / 2 ) --->   APB2 clock = 137.5 MHz
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;

	// APB4   clock (D3PCLK1) prescaler:
	// -> D3PPRE         = ( 1 / 2 ) --->   APB4 clock = 137.5 MHz
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

	// Inicializar los relojes.
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
}
