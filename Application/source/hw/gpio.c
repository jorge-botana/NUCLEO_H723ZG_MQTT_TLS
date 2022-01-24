// ------------------------------------------------------------------------------
// GPIO
//
// - Lectura del pulsador y encendido/apagado de los LEDs.
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


#include "gpio.h"


#define   B1_CLK_EN()                                __HAL_RCC_GPIOC_CLK_ENABLE()
#define   B1_PORT                                                           GPIOC
#define   B1_PIN                                                      GPIO_PIN_13

#define   LD1_CLK_EN()                               __HAL_RCC_GPIOB_CLK_ENABLE()
#define   LD1_PORT                                                          GPIOB
#define   LD1_PIN                                                      GPIO_PIN_0

#define   LD2_CLK_EN()                               __HAL_RCC_GPIOE_CLK_ENABLE()
#define   LD2_PORT                                                          GPIOE
#define   LD2_PIN                                                      GPIO_PIN_1

#define   LD3_CLK_EN()                               __HAL_RCC_GPIOB_CLK_ENABLE()
#define   LD3_PORT                                                          GPIOB
#define   LD3_PIN                                                     GPIO_PIN_14


void GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	B1_CLK_EN();
	GPIO_InitStruct.Pin   = B1_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	HAL_GPIO_Init(B1_PORT, &GPIO_InitStruct);

	LD1_CLK_EN();
	GPIO_InitStruct.Pin   = LD1_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(LD1_PORT, &GPIO_InitStruct);

	LD2_CLK_EN();
	GPIO_InitStruct.Pin   = LD2_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(LD2_PORT, &GPIO_InitStruct);

	LD3_CLK_EN();
	GPIO_InitStruct.Pin   = LD3_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(LD3_PORT, &GPIO_InitStruct);
}


int B1_Read(void)
{
	return HAL_GPIO_ReadPin(B1_PORT, B1_PIN);
}


void LD1_On(void)
{
	HAL_GPIO_WritePin(LD1_PORT, LD1_PIN, GPIO_PIN_SET);
}


void LD1_Off(void)
{
	HAL_GPIO_WritePin(LD1_PORT, LD1_PIN, GPIO_PIN_RESET);
}


void LD2_On(void)
{
	HAL_GPIO_WritePin(LD2_PORT, LD2_PIN, GPIO_PIN_SET);
}


void LD2_Off(void)
{
	HAL_GPIO_WritePin(LD2_PORT, LD2_PIN, GPIO_PIN_RESET);
}


void LD3_On(void)
{
	HAL_GPIO_WritePin(LD3_PORT, LD3_PIN, GPIO_PIN_SET);
}


void LD3_Off(void)
{
	HAL_GPIO_WritePin(LD3_PORT, LD3_PIN, GPIO_PIN_RESET);
}
