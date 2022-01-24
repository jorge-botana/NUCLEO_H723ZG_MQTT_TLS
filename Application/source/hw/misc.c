// ------------------------------------------------------------------------------
// MISC
//
// - Inicialización de la caché y del driver HAL.
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


#include "misc.h"


void CACHE_Init(void)
{
	// Habilitar la caché mejora el rendimiento del programa, y eso se puede ver
	// en los tiempos de escritura y lectura de paquetes en el modo de pruebas,
	// que pasan a ser mucho más pequeños.

	SCB_EnableICache(); // Caché de instrucciones
	SCB_EnableDCache(); // Caché de datos
}


void SYS_Init(void)
{
	HAL_Init();
}


void HAL_MspInit(void)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE();
}
