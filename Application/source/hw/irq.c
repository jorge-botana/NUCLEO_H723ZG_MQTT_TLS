// ------------------------------------------------------------------------------
// IRQ
//
// - Interrupciones y excepciones.
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
#include "ethernetif.h"


// --------------- Interrupciones y excepciones del ARM Cortex-M7 ---------------


void NMI_Handler(void)
{
	for( ;; );
}


void HardFault_Handler(void)
{
	for( ;; );
}


void MemManage_Handler(void)
{
	for( ;; );
}


void BusFault_Handler(void)
{
	for( ;; );
}


void UsageFault_Handler(void)
{
	for( ;; );
}


// void SVC_Handler(void)
// {
// 	for( ;; ); // Ya implementado por FreeRTOS.
// }


void DebugMon_Handler(void)
{
	for( ;; );
}


// void PendSV_Handler(void)
// {
// 	for( ;; ); // Ya implementado por FreeRTOS.
// }


// void SysTick_Handler(void)
// {
// 	for( ;; ); // Ya implementado por CMSIS-RTOS2.
// }


// ----------------------- Interrupciones de periféricos ------------------------


void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim6);
}


void TIM7_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim7);
}


void ETH_IRQHandler(void)
{
	HAL_ETH_IRQHandler(&heth);
}
