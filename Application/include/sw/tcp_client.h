// ------------------------------------------------------------------------------
// TCP_CLIENT
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

#ifndef   TCP_CLIENT_H
#define   TCP_CLIENT_H

#include <stddef.h>

extern int s;

int TCP_Init(const char *hostname, const char *ipaddr, const int port);
int TCP_Close(char *buf, size_t len);

int TCP_Read(char *buf, size_t len);
int TCP_Write(const char *buf, size_t len);

#endif // TCP_CLIENT_H
