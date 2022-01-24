// ------------------------------------------------------------------------------
// RNG
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

#ifndef   RNG_H
#define   RNG_H

#include  HAL_DRIVER

void RNG_Init(void);

unsigned int RNG_Get(void);

#endif // RNG_H
