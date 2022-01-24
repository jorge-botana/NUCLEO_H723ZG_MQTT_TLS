// ------------------------------------------------------------------------------
// RNG
//
// - Generación de números aleatorios.
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


#include "rng.h"


RNG_HandleTypeDef hrng = {0};


void RNG_Init(void)
{
	hrng.Instance = RNG;
	HAL_RNG_Init(&hrng);
}


unsigned int RNG_Get(void)
{
	unsigned int random = 0;
	HAL_RNG_GenerateRandomNumber(&hrng, (uint32_t *)&random);

	return random;
}


void HAL_RNG_MspInit(RNG_HandleTypeDef *hrng)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	if(hrng->Instance == RNG)
	{
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RNG;
		PeriphClkInitStruct.RngClockSelection    = RCC_RNGCLKSOURCE_HSI48;
		HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		__HAL_RCC_RNG_CLK_ENABLE();
	}
}
