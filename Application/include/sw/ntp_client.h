// ------------------------------------------------------------------------------
// NTP_CLIENT
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

#ifndef   NTP_CLIENT_H
#define   NTP_CLIENT_H

#include <time.h>

int NTP_Request(const char *hostname, const char *ipaddr, const int gmt_time, time_t *curtime);

#endif // NTP_CLIENT_H
