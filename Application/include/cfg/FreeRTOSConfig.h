// ------------------------------------------------------------------------------
// FREERTOSCONFIG
//
// - Archivo de configuración de FreeRTOS.
//
// ------------------------------------------------------------------------------
// FreeRTOS Kernel V10.4.4
// Copyright (C) 2021 Amazon.com, Inc. or its affiliates.
// All Rights Reserved.
//
// SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// https://www.FreeRTOS.org
// https://github.com/FreeRTOS
// ------------------------------------------------------------------------------

#ifndef   FREERTOSCONFIG_H
#define   FREERTOSCONFIG_H

// ---------------------------- Dispositivo : STM32 -----------------------------

#include  STM32_DEVICE

// ----------------------- Port para GCC : ARM Cortex-M7 ------------------------

#define   configCPU_CLOCK_HZ                                      SystemCoreClock
#define   configTICK_RATE_HZ                                                 1000

#define   configPRIO_BITS                                        __NVIC_PRIO_BITS
#define   configLIBRARY_LOWEST_INTERRUPT_PRIORITY                              15
#define   configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY                          5

#define   configKERNEL_INTERRUPT_PRIORITY                                       \
(                                                                               \
	configLIBRARY_LOWEST_INTERRUPT_PRIORITY      <<                             \
	(8 - configPRIO_BITS)                                                       \
)

#define   configMAX_SYSCALL_INTERRUPT_PRIORITY                                  \
(                                                                               \
	configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY <<                             \
	(8 - configPRIO_BITS)                                                       \
)

#define   vPortSVCHandler                                             SVC_Handler
#define   xPortPendSVHandler                                       PendSV_Handler

// ------------------- Esquema de gestión de memoria : heap_4 -------------------

#define   configMINIMAL_STACK_SIZE                                           1024
#define   configTOTAL_HEAP_SIZE                   (25 * configMINIMAL_STACK_SIZE)

// --------------------------- Wrapper : CMSIS-RTOS2 ----------------------------

#define   INCLUDE_xSemaphoreGetMutexHolder                                      1
#define   INCLUDE_vTaskDelay                                                    1
#define   INCLUDE_vTaskDelayUntil                                               1
#define   INCLUDE_vTaskDelete                                                   1
#define   INCLUDE_xTaskGetCurrentTaskHandle                                     1
#define   INCLUDE_xTaskGetSchedulerState                                        1
#define   INCLUDE_uxTaskGetStackHighWaterMark                                   1
#define   INCLUDE_uxTaskPriorityGet                                             1
#define   INCLUDE_vTaskPrioritySet                                              1
#define   INCLUDE_eTaskGetState                                                 1
#define   INCLUDE_vTaskSuspend                                                  1
#define   INCLUDE_xTimerPendFunctionCall                                        1

#define   configUSE_TIMERS                                                      1
#define   configUSE_MUTEXES                                                     1
#define   configUSE_COUNTING_SEMAPHORES                                         1
#define   configUSE_TRACE_FACILITY                                              1
#define   configUSE_16_BIT_TICKS                                                0
#define   configMAX_PRIORITIES                                                 56
#define   configUSE_PORT_OPTIMISED_TASK_SELECTION                               0

#define   configTIMER_TASK_PRIORITY                                             2
#define   configTIMER_QUEUE_LENGTH                                             10
#define   configTIMER_TASK_STACK_DEPTH                   configMINIMAL_STACK_SIZE

#define   configUSE_PREEMPTION                                                  1
#define   configUSE_IDLE_HOOK                                                   0
#define   configUSE_TICK_HOOK                                                   0

// ------------------------------------------------------------------------------

#endif // FREERTOSCONFIG_H
