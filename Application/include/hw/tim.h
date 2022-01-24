// ------------------------------------------------------------------------------
// TIM
//
// - Archivo de cabecera.
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

#ifndef   TIM_H
#define   TIM_H

#include  HAL_DRIVER

extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;

void Tick_Delay(unsigned int delay_ms);

void TIM_Init(void);

void TIM_Start(void);
void TIM_Stop(void);
int TIM_Read(void);

#endif // TIM_H
