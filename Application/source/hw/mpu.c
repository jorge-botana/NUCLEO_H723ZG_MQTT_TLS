// ------------------------------------------------------------------------------
// MPU
//
// - Inicialización de la unidad de protección de memoria en RAM_D2:
//
//   -> SRAM1: 0x30000000 - 0x30003FFF : ETH DMA desc. + Rx buffers
//   -> SRAM2: 0x30004000 - 0x30007FFF : Tx buffers (lwIP RAM heap)
//
//   RAM_D2 = SRAM1 + SRAM2 = 16 KB + 16 KB = 32 KB
//
//   Esto es necesario en los microcontroladores STM32H723 para que la D-Cache no
//   haga uso de esta zona de memoria, ya que es necesario reservarla para los
//   descriptores DMA de Ethernet y sus buffers.
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


#include "mpu.h"


void MPU_Init(void)
{
#ifdef    STM32H723xx
	MPU_Region_InitTypeDef MPU_InitStruct = {0};

	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
	MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
	MPU_InitStruct.BaseAddress      = 0x30000000;
	MPU_InitStruct.Size             = MPU_REGION_SIZE_32KB;
	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
#endif // STM32H723xx
}
