// ------------------------------------------------------------------------------
// SW
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

#ifndef   SW_H
#define   SW_H

#include <time.h>

void Sw_TCPIP_Init(void);

int Sw_DHCP_Request(const int dhcp_attempts, const char *static_ip, const char *static_netmask, const char *static_gw, char *gateway);

int Sw_DNS_Request(const char *server, const char *hostname, size_t len, char *ipaddr);

int Sw_NTP_Request(const char *hostname, const char *ipaddr, const int gmt_time, time_t *curtime);

int Sw_MQTT_Init(const char *hostname, const char *ipaddr, const int port);
int Sw_MQTT_Close(char *buf, size_t len);

int Sw_MQTT_Read_Packet(char *buf, size_t len);

int Sw_MQTT_Write_CONNECT(const char *client, size_t c_len, const int keep_alive);
int Sw_MQTT_Write_DISCONNECT(void);
int Sw_MQTT_Write_PUBLISH(const char *topic, size_t t_len, const char *msg, size_t m_len);
int Sw_MQTT_Write_SUBSCRIBE(const char *topic, size_t t_len);
int Sw_MQTT_Write_UNSUBSCRIBE(const char *topic, size_t t_len);
int Sw_MQTT_Write_PINGREQ(void);

#endif // SW_H
