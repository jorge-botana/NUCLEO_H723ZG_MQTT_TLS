// ------------------------------------------------------------------------------
// DHCP_CLIENT
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

#ifndef   DHCP_CLIENT_H
#define   DHCP_CLIENT_H

int DHCP_Request(const int dhcp_attempts, const char *static_ip, const char *static_netmask, const char *static_gw, char *gateway);

#endif // DHCP_CLIENT_H
