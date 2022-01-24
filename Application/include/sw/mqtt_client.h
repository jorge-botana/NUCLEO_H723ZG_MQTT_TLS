// ------------------------------------------------------------------------------
// MQTT_CLIENT
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

#ifndef   MQTT_CLIENT_H
#define   MQTT_CLIENT_H

#include <stddef.h>

int MQTT_Init(const char *hostname, const char *ipaddr, const int port);
int MQTT_Close(char *buf, size_t len);

int MQTT_Read_Packet(char *buf, size_t len);

int MQTT_Write_CONNECT(const char *client, size_t c_len, const int keep_alive);
int MQTT_Write_PUBLISH(const char *topic, size_t t_len, const char *msg, size_t m_len);
int MQTT_Write_SUBSCRIBE(const char *topic, size_t t_len);
int MQTT_Write_UNSUBSCRIBE(const char *topic, size_t t_len);
int MQTT_Write_PINGREQ(void);
int MQTT_Write_DISCONNECT(void);

#endif // MQTT_CLIENT_H
