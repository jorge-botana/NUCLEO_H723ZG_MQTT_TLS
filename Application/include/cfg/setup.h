// ------------------------------------------------------------------------------
// SETUP
//
// - Carga del archivo de configuración del programa.
//
// - Para las definiciones que tengan que llevar asignado un valor, si alguna de
//   ellas no estuviera definida en el archivo de configuración del programa, se
//   cargará un valor predeterminado.
//
// - Carga de definiciones adicionales a partir de la configuración anterior.
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

#ifndef   SETUP_H
#define   SETUP_H

// ------------------- Archivo de configuración del programa --------------------

#include "settings.h"

// -------------------------- Valores predeterminados ---------------------------

#ifndef   PLATFORM
#define   PLATFORM                                                "[MY_PLATFORM]"
#endif // PLATFORM

#ifndef   DHCP_ATTEMPTS
#define   DHCP_ATTEMPTS                                                        50
#endif // DHCP_ATTEMPTS

#ifndef   STATIC_IP
#define   STATIC_IP                                                "192.168.1.10"
#endif // STATIC_IP

#ifndef   STATIC_NETMASK
#define   STATIC_NETMASK                                          "255.255.255.0"
#endif // STATIC_NETMASK

#ifndef   STATIC_GW
#define   STATIC_GW                                                 "192.168.1.1"
#endif // STATIC_GW

#ifndef   MAIN_DNS_SERVER
#define   MAIN_DNS_SERVER                                                 gateway
#endif // MAIN_DNS_SERVER

#ifndef   ALT_DNS_SERVER
#define   ALT_DNS_SERVER                                                "1.1.1.1"
#endif // ALT_DNS_SERVER

#ifndef   MAIN_NTP_SERVER
#define   MAIN_NTP_SERVER                                    "MY-MAIN-NTP-SERVER"
#endif // MAIN_NTP_SERVER

#ifndef   ALT_NTP_SERVER
#define   ALT_NTP_SERVER                                        "es.pool.ntp.org"
#endif // ALT_NTP_SERVER

#ifndef   GMT_TIME
#define   GMT_TIME                                                              0
#endif // GMT_TIME

#ifndef   TLS_DEBUG_LEVEL
#define   TLS_DEBUG_LEVEL                                                       0
#endif // TLS_DEBUG_LEVEL

#ifndef   MAIN_MQTT_BROKER
#define   MAIN_MQTT_BROKER                                  "MY-MAIN-MQTT-BROKER"
#endif // MAIN_MQTT_BROKER

#ifndef   ALT_MQTT_BROKER
#define   ALT_MQTT_BROKER                                    "test.mosquitto.org"
#endif // ALT_MQTT_BROKER

#ifndef   CLIENT_ID
#define   CLIENT_ID                                              "[MY_CLIENT_ID]"
#endif // CLIENT_ID

#ifndef   KEEP_ALIVE
#define   KEEP_ALIVE                                                            0
#endif // KEEP_ALIVE

#ifndef   PUBLIC_TOPIC
#define   PUBLIC_TOPIC                                        "[MY_PUBLIC_TOPIC]"
#endif // PUBLIC_TOPIC

#ifndef   SECRET_TOPIC
#define   SECRET_TOPIC                                        "[MY_SECRET_TOPIC]"
#endif // SECRET_TOPIC

#ifndef   TEST_MSG_BYTES
#define   TEST_MSG_BYTES                                                     1024
#endif // TEST_MSG_BYTES

#ifndef   TEST_N_CYCLES
#define   TEST_N_CYCLES                                                       100
#endif // TEST_N_CYCLES

// -------------------------- Definiciones adicionales --------------------------

#ifdef    USE_MAIN_DNS_SERVER
#define   DNS_SERVER                                              MAIN_DNS_SERVER
#else  // USE_MAIN_DNS_SERVER
#define   DNS_SERVER                                               ALT_DNS_SERVER
#endif // USE_MAIN_DNS_SERVER

#ifdef    USE_MAIN_NTP_SERVER
#define   NTP_SERVER                                              MAIN_NTP_SERVER
#else  // USE_MAIN_NTP_SERVER
#define   NTP_SERVER                                               ALT_NTP_SERVER
#endif // USE_MAIN_NTP_SERVER

#ifdef    USE_MAIN_MQTT_BROKER
#define   MQTT_BROKER                                            MAIN_MQTT_BROKER
#else  // USE_MAIN_MQTT_BROKER
#define   MQTT_BROKER                                             ALT_MQTT_BROKER
#endif // USE_MAIN_MQTT_BROKER

#ifdef    USE_TLS
#ifdef    USE_CLIENT_AUTH
#define   MQTT_PORT                                                          8884
#else  // USE_CLIENT_AUTH
#define   MQTT_PORT                                                          8883
#endif // USE_CLIENT_AUTH
#else  // USE_TLS
#define   MQTT_PORT                                                          1883
#endif // USE_TLS

// ------------------------------------------------------------------------------

#endif // SETUP_H
