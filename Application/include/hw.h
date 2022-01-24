// ------------------------------------------------------------------------------
// HW
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

#ifndef   HW_H
#define   HW_H

#include <time.h>

void Hw_Init(void);

void Hw_Tick_Delay(unsigned int delay_ms);

int Hw_B1_Read(void);

void Hw_LD1_On(void);
void Hw_LD1_Off(void);

void Hw_LD2_On(void);
void Hw_LD2_Off(void);

void Hw_LD3_On(void);
void Hw_LD3_Off(void);

unsigned int Hw_RNG_Get(void);

time_t Hw_RTC_Read(void);
void Hw_RTC_Write(time_t curtime);

void Hw_UART_Start(void);
void Hw_UART_Stop(void);

void Hw_TIM_Start(void);
void Hw_TIM_Stop(void);
int Hw_TIM_Read(void);

int Hw_ETH_Link_Status(void);

#endif // HW_H
