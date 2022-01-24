// ------------------------------------------------------------------------------
// TIM
//
// - Temporizador (TIM6) usado por el driver HAL y que reemplaza al SysTick, ya
//   que este último lo utiliza el RTOS (el driver HAL y el RTOS no deberían
//   compartir fuente de reloj). Este temporizador también provee delays
//   bloqueantes. Un segundo temporizador (TIM7) es usado para medir tiempos en
//   microsegundos.
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


#include "tim.h"


#define   TIM7_FREQUENCY_MHZ                                                  275
#define   TIM7_PERIOD_US                                                        5
#define   TIM7_PSC                                                              0
#define   TIM7_ARR                      (TIM7_FREQUENCY_MHZ - 1) * TIM7_PERIOD_US


TIM_HandleTypeDef htim6 = {0};
TIM_HandleTypeDef htim7 = {0};

volatile int us_counter = 0;


HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
	RCC_ClkInitTypeDef clkconfig       = {0};
	uint32_t           pFLatency       = 0;
	uint32_t           uwAPB1Prescaler = 0;
	uint32_t           uwTimclock      = 0;

	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, TickPriority, 0);
	HAL_NVIC_EnableIRQ  (TIM6_DAC_IRQn);
	uwTickPrio = TickPriority;

	__HAL_RCC_TIM6_CLK_ENABLE();

	HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

	uwAPB1Prescaler = clkconfig.APB1CLKDivider;

	if(uwAPB1Prescaler == RCC_HCLK_DIV1)
	{
		uwTimclock  =     HAL_RCC_GetPCLK1Freq();
	}
	else
	{
		uwTimclock  = 2 * HAL_RCC_GetPCLK1Freq();
	}

	htim6.Instance         = TIM6;
	htim6.Init.Prescaler   = (uwTimclock / 1000000) - 1;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period      = (   1000000 /    1000) - 1;
	HAL_TIM_Base_Init(&htim6);

	return HAL_TIM_Base_Start_IT(&htim6);
}


void Tick_Delay(unsigned int delay_ms)
{
	HAL_Delay(delay_ms);
}


void TIM_Init(void)
{
	htim7.Instance               = TIM7;
	htim7.Init.Prescaler         = TIM7_PSC;
	htim7.Init.CounterMode       = TIM_COUNTERMODE_UP;
	htim7.Init.Period            = TIM7_ARR;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&htim7);
}


void TIM_Start(void)
{
	us_counter = 0;
	HAL_TIM_Base_Start_IT(&htim7);
}


void TIM_Stop(void)
{
	HAL_TIM_Base_Stop_IT(&htim7);
}


int TIM_Read(void)
{
	return us_counter;
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM7)
	{
		__HAL_RCC_TIM7_CLK_ENABLE();

		HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ  (TIM7_IRQn);
	}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM6)
	{
		HAL_IncTick();
	}

	if(htim->Instance == TIM7)
	{
		us_counter = us_counter + TIM7_PERIOD_US;
	}
}


// ------------------------------------------------------------------------------
// Fórmula empleada para calcular la frecuencia de un temporizador:
//
// -> f (Hz) = Timer clock    Hz / [(TIM_PSC + 1) * (TIM_ARR + 1)]
//
// Ejemplo para un temporizador de 1 Hz (periodo de 1 segundo):
//
// -> 1 (Hz) = 275 * (10 ^ 6) Hz / [(  9 999 + 1) * ( 27 499 + 1)]
//
// En este caso el temporizador interrumpiría cada segundo.
// ------------------------------------------------------------------------------
