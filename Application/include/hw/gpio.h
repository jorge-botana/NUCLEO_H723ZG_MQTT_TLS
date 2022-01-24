// ------------------------------------------------------------------------------
// GPIO
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

#ifndef   GPIO_H
#define   GPIO_H

#include  HAL_DRIVER

void GPIO_Init(void);

int B1_Read(void);

void LD1_On(void);
void LD1_Off(void);

void LD2_On(void);
void LD2_Off(void);

void LD3_On(void);
void LD3_Off(void);

#endif // GPIO_H
