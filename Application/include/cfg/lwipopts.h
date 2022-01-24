// ------------------------------------------------------------------------------
// LWIPOPTS
//
// - Archivo de configuración de lwIP.
//
// ------------------------------------------------------------------------------
// Copyright (c) 2001-2004 Swedish Institute of Computer Science.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
// ------------------------------------------------------------------------------

#ifndef   LWIPOPTS_H
#define   LWIPOPTS_H

#ifdef    STM32H723xx
// ------------------------------------------------------------------------------
// Realocar el puntero del heap de lwIP. Esto es necesario en los
// microcontroladores STM32H723, ya que sus descriptores DMA de Ethernet no
// pueden acceder a cualquier zona de la memoria RAM.
// ------------------------------------------------------------------------------
#define   LWIP_RAM_HEAP_POINTER                                        0x30004000
#endif // STM32H723xx

// ------------------------------------------------------------------------------
// Establecer la alineación de la memoria (CPU 32 bits = 4 bytes).
// ------------------------------------------------------------------------------
#define   MEM_ALIGNMENT                                                         4

// ------------------------------------------------------------------------------
// Aumentar el tamaño del heap de lwIP para poder enviar paquetes de gran tamaño
// (el predeterminado es de solo 1600 bytes).
// ------------------------------------------------------------------------------
#define   MEM_SIZE                                                     (4 * 1024)

#ifdef    STM32H723xx
// ------------------------------------------------------------------------------
// Deshabilitar el "PBUF pool" para ahorrar mucha memoria RAM, ya que no se usa
// porque el driver de Ethernet empleado en un STM32H723 usa un "Zero-copy RX
// PBUF pool" en su lugar.
// ------------------------------------------------------------------------------
#define   PBUF_POOL_SIZE                                                        0
#endif // STM32H723xx

// ------------------------------------------------------------------------------
// Habilitar el módulo de DHCP integrado en lwIP.
// ------------------------------------------------------------------------------
#define   LWIP_DHCP                                                             1

// ------------------------------------------------------------------------------
// Aumentar el "TCP Maximum Segment Size" para ajustarlo al "Maximum Transmission
// Unit" (MTU) de Ethernet:
//
// -> 1500 bytes MTU - 20 bytes IP - 20 bytes TCP = 1460 bytes MSS
// ------------------------------------------------------------------------------
#define   TCP_MSS                                                            1460

// ------------------------------------------------------------------------------
// Configurar la tarea de TCP/IP.
// ------------------------------------------------------------------------------
#define   TCPIP_THREAD_STACKSIZE                   (2 * configMINIMAL_STACK_SIZE)
#define   TCPIP_THREAD_PRIO                                        osPriorityHigh
#define   TCPIP_MBOX_SIZE                                                       6
#define   DEFAULT_UDP_RECVMBOX_SIZE                                             6
#define   DEFAULT_TCP_RECVMBOX_SIZE                                             6

// ------------------------------------------------------------------------------
// Habilitar un timeout para las operaciones de lectura en sockets.
// ------------------------------------------------------------------------------
#define   LWIP_SO_RCVTIMEO                                                      1

// ------------------------------------------------------------------------------

#endif // LWIPOPTS_H
