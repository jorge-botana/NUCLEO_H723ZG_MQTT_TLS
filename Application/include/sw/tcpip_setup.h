// ------------------------------------------------------------------------------
// TCPIP_SETUP
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

#ifndef   TCPIP_SETUP_H
#define   TCPIP_SETUP_H

#include "lwip/netif.h"

extern struct netif netif;

err_t ethernetif_init(struct netif *netif);

void TCPIP_Init(void);

void TCPIP_Down_Up(void);

#endif // TCPIP_SETUP_H
