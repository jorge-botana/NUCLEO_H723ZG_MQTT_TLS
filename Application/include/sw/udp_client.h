// ------------------------------------------------------------------------------
// UDP_CLIENT
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

#ifndef   UDP_CLIENT_H
#define   UDP_CLIENT_H

#include <stddef.h>

int UDP_Request(const char *hostname, const char *ipaddr, const int port, const char *s_buf, size_t s_len, char *r_buf, size_t r_len);

#endif // UDP_CLIENT_H
