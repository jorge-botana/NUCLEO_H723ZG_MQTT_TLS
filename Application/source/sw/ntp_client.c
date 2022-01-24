// ------------------------------------------------------------------------------
// NTP_CLIENT
//
// - Cliente NTP v4.
//
// - Requiere de UDP_CLIENT.
//
// - No emplea librerías NTP de terceros.
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


#include "ntp_client.h"

#include "udp_client.h"

#include <stdio.h>


#define   NTP_PORT                                                            123
#define   NTP_PACKET_SIZE                                                      48
#define   SECS_FROM_1900_TO_1970                                       2208988800


int NTP_Request(const char *hostname, const char *ipaddr, const int gmt_time, time_t *curtime)
{
	printf(" ---------- Cliente NTP -----------\n\n");

	// Generar el paquete NTP.
	char NTP_REQUEST[NTP_PACKET_SIZE] = {0};

	// Modificar únicamente el primer byte del paquete NTP. Los otros bytes
	// pueden valer 0.
	NTP_REQUEST[0] = 0b11100011;
	               //  76|||||| - Leap Indicator: unknown (clock unsynchronized)
	               //    ||||||   (3)
	               //    543||| - Version number: NTP Version 4 (4)
	               //       210 - Mode: client (3)

	// Realizar la comunicación.
	char recv_buf[NTP_PACKET_SIZE] = {0};
	int ret = UDP_Request(hostname, ipaddr, NTP_PORT, NTP_REQUEST, NTP_PACKET_SIZE, recv_buf, sizeof(recv_buf));
	if(ret < 0)
	{
		printf(" - No se pudo obtener la fecha y hora\n\n");
		return ret;
	}

	// Obtener los segundos desde el 01-01-1900 (contenidos en los bytes [40, 41,
	// 42, 43] del paquete recibido), el origen de tiempos que usa NTP (UTC).
	unsigned long secs_since_1900 = \
		(recv_buf[40] << 24) +      \
		(recv_buf[41] << 16) +      \
		(recv_buf[42] <<  8) +      \
		(recv_buf[43] <<  0);

	// Obtener los segundos desde el 01-01-1970, el origen de tiempos que usan
	// las funciones de "time.h" (UTC).
	*curtime = (time_t)(secs_since_1900 - SECS_FROM_1900_TO_1970);

	// Actualizar a la fecha y hora local (GMT).
	*curtime = *curtime + (gmt_time * 60 * 60);

	// Mostrar la fecha y hora.
	char buf[30] = {0};
	strftime(buf, sizeof(buf), "%a %d-%m-%Y - %H:%M:%S", localtime(curtime));
	printf(" -> Date & Time : %s\n", buf);
	printf(" -> Time zone   : GMT%+i\n\n", gmt_time);

	return 0;
}
