// ------------------------------------------------------------------------------
// UART
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

#ifndef   UART_H
#define   UART_H

#include  HAL_DRIVER

void UART_Init(void);

void UART_Start(void);
void UART_Stop(void);

#endif // UART_H
