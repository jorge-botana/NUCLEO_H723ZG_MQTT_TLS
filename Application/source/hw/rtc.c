// ------------------------------------------------------------------------------
// RTC
//
// - Lectura y escritura del reloj de tiempo real.
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


#include "rtc.h"


RTC_HandleTypeDef hrtc = {0};


void RTC_Init(void)
{
	hrtc.Instance          = RTC;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv  = 255;
	HAL_RTC_Init(&hrtc);
}


time_t RTC_Read(void)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	struct tm s_time_date = {0};

	// Advertencia: las lecturas deben hacerse en el siguiente orden: primero
	// HAL_RTC_GetTime() y luego HAL_RTC_GetDate(). De lo contrario, no se va a
	// leer correctamente el RTC.
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	s_time_date.tm_hour = sTime.Hours;
	s_time_date.tm_min  = sTime.Minutes;
	s_time_date.tm_sec  = sTime.Seconds;

	s_time_date.tm_wday = sDate.WeekDay;
	s_time_date.tm_mon  = sDate.Month;
	s_time_date.tm_mday = sDate.Date;
	s_time_date.tm_year = sDate.Year;

	return mktime(&s_time_date);
}


void RTC_Write(time_t curtime)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	struct tm s_time_date = {0};

	localtime_r(&curtime, &s_time_date);

	sTime.Hours   = s_time_date.tm_hour;
	sTime.Minutes = s_time_date.tm_min;
	sTime.Seconds = s_time_date.tm_sec;

	sDate.WeekDay = s_time_date.tm_wday;
	sDate.Month   = s_time_date.tm_mon;
	sDate.Date    = s_time_date.tm_mday;
	sDate.Year    = s_time_date.tm_year;

	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}


void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	if(hrtc->Instance == RTC)
	{
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
		PeriphClkInitStruct.RTCClockSelection    = RCC_RTCCLKSOURCE_LSE;
		HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		__HAL_RCC_RTC_ENABLE();
	}
}
