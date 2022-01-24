// ------------------------------------------------------------------------------
// DNS_CLIENT
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

#ifndef   DNS_CLIENT_H
#define   DNS_CLIENT_H

#include <stddef.h>

int DNS_Request(const char *server, const char *hostname, size_t len, char *ipaddr);

#endif // DNS_CLIENT_H
