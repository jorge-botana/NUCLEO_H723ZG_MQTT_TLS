// ------------------------------------------------------------------------------
// HW
//
// - API que contiene funciones que hacen uso de periféricos.
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


#include "hw.h"

#include "mpu.h"
#include "misc.h"
#include "rcc.h"
#include "gpio.h"
#include "rng.h"
#include "rtc.h"
#include "uart.h"
#include "tim.h"
#include "ethernetif.h"


// ------------------------------------------------------------------------------
// Inicializa todos los periféricos usados excepto Ethernet, ya que este es
// inicializado por el stack de TCP/IP.
//
// -> (void    ) Nada.
//
// <- (void    ) Nada.
// ------------------------------------------------------------------------------
void Hw_Init(void)
{
	MPU_Init  ();
	CACHE_Init();
	SYS_Init  ();
	RCC_Init  ();
	GPIO_Init ();
	RNG_Init  ();
	RTC_Init  ();
	UART_Init ();
	TIM_Init  ();
}


// ------------------------------------------------------------------------------
// Bloquea durante el tiempo especificado.
//
// -> (void    ) Número de milisegundos.
//
// <- (void    ) Nada.
// ------------------------------------------------------------------------------
void Hw_Tick_Delay(unsigned int delay_ms)
{
	Tick_Delay(delay_ms);
}


// ------------------------------------------------------------------------------
// Lee el pulsador.
//
// -> (void    ) Nada.
//
// <- (int     ) 1 si está pulsado, 0 si no lo está.
// ------------------------------------------------------------------------------
int Hw_B1_Read(void)
{
	return B1_Read();
}


// ------------------------------------------------------------------------------
// Enciende el LED 1.
//
// -> (void    ) Nada.
//
// <- (void    ) Nada.
// ------------------------------------------------------------------------------
void Hw_LD1_On(void)
{
	LD1_On();
}


// ------------------------------------------------------------------------------
// Apaga el LED 1.
//
// -> (void    ) Nada.
//
// <- (void    ) Nada.
// ------------------------------------------------------------------------------
void Hw_LD1_Off(void)
{
	LD1_Off();
}


// ------------------------------------------------------------------------------
// Enciende el LED 2.
//
// -> (void    ) Nada.
//
// <- (void    ) Nada.
// ------------------------------------------------------------------------------
void Hw_LD2_On(void)
{
	LD2_On();
}


// ------------------------------------------------------------------------------
// Apaga el LED 2.
//
// -> (void    ) Nada.
//
// <- (void    ) Nada.
// ------------------------------------------------------------------------------
void Hw_LD2_Off(void)
{
	LD2_Off();
}


// ------------------------------------------------------------------------------
// Enciende el LED 3.
//
// -> (void    ) Nada.
//
// <- (void    ) Nada.
// ------------------------------------------------------------------------------
void Hw_LD3_On(void)
{
	LD3_On();
}


// ------------------------------------------------------------------------------
// Apaga el LED 3.
//
// -> (void    ) Nada.
//
// <- (void    ) Nada.
// ------------------------------------------------------------------------------
void Hw_LD3_Off(void)
{
	LD3_Off();
}


// ------------------------------------------------------------------------------
// Obtiene un número aleatorio.
//
// -> (void    ) Nada.
//
// <- (uint    ) Número aleatorio.
// ------------------------------------------------------------------------------
unsigned int Hw_RNG_Get(void)
{
	return RNG_Get();
}


// ------------------------------------------------------------------------------
// Lee la fecha y hora del RTC.
//
// -> (void    ) Nada.
//
// <- (time_t  ) Timestamp.
// ------------------------------------------------------------------------------
time_t Hw_RTC_Read(void)
{
	return RTC_Read();
}


// ------------------------------------------------------------------------------
// Escribe la fecha y hora en el RTC.
//
// -> (time_t  ) Timestamp.
//
// <- (void    ) Nada.
// ------------------------------------------------------------------------------
void Hw_RTC_Write(time_t curtime)
{
	RTC_Write(curtime);
}


// ------------------------------------------------------------------------------
// Activa la UART.
//
// -> (void    ) Nada.
//
// <- (void    ) Nada.
// ------------------------------------------------------------------------------
void Hw_UART_Start(void)
{
	UART_Start();
}


// ------------------------------------------------------------------------------
// Desactiva la UART.
//
// -> (void    ) Nada.
//
// <- (void    ) Nada.
// ------------------------------------------------------------------------------
void Hw_UART_Stop(void)
{
	UART_Stop();
}


// ------------------------------------------------------------------------------
// Reinicia y arranca el temporizador.
//
// -> (void    ) Nada.
//
// <- (void    ) Nada.
// ------------------------------------------------------------------------------
void Hw_TIM_Start(void)
{
	TIM_Start();
}


// ------------------------------------------------------------------------------
// Para el temporizador.
//
// -> (void    ) Nada.
//
// <- (void    ) Nada.
// ------------------------------------------------------------------------------
void Hw_TIM_Stop(void)
{
	TIM_Stop();
}


// ------------------------------------------------------------------------------
// Lee la cuenta actual del temporizador.
//
// -> (void    ) Nada.
//
// <- (int     ) Número de microsegundos.
// ------------------------------------------------------------------------------
int Hw_TIM_Read(void)
{
	return TIM_Read();
}


// ------------------------------------------------------------------------------
// Comprueba el estado de la conexión LAN.
//
// -> (void    ) Nada.
//
// <- (int     ) 0 si se detecta la conexión, un valor negativo en caso
//               contrario.
// ------------------------------------------------------------------------------
int Hw_ETH_Link_Status(void)
{
	return ETH_Link_Status();
}
