// ------------------------------------------------------------------------------
// SETTINGS
//
// - Archivo de configuración del programa.
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

#ifndef   SETTINGS_H
#define   SETTINGS_H

// ---------------------------------- General -----------------------------------

// ------------------------------------------------------------------------------
// Nombre de la plataforma sobre la que corre el programa.
//
// Valor predeterminado :                                         "[MY_PLATFORM]"
// ------------------------------------------------------------------------------
#define   PLATFORM                                                 STM32_PLATFORM

// ------------------------------------------------------------------------------
// - Definido  : habilitar la depuración por la UART.
//
// - Comentado : deshabilitar la depuración por la UART.
// ------------------------------------------------------------------------------
#define   UART_DEBUG

// ------------------------------------ DHCP ------------------------------------

// ------------------------------------------------------------------------------
// Número máximo de intentos del cliente DHCP hasta establecer una configuración
// de red estática. Se puede asignar el valor 0 si se desea esto último.
//
// Advertencia: NO es el número máximo de paquetes "DHCP Request" enviados.
//
// Valor predeterminado :                                                      50
// ------------------------------------------------------------------------------
#define   DHCP_ATTEMPTS                                                        50

// ------------------------------------------------------------------------------
// IP estática.
//
// Valor predeterminado :                                          "192.168.1.10"
// ------------------------------------------------------------------------------
#define   STATIC_IP                                                "192.168.1.10"

// ------------------------------------------------------------------------------
// Máscara de subred estática.
//
// Valor predeterminado :                                         "255.255.255.0"
// ------------------------------------------------------------------------------
#define   STATIC_NETMASK                                          "255.255.255.0"

// ------------------------------------------------------------------------------
// Puerta de enlace estática.
//
// Valor predeterminado :                                           "192.168.1.1"
// ------------------------------------------------------------------------------
#define   STATIC_GW                                                 "192.168.1.1"

// ------------------------------------ DNS -------------------------------------

// ------------------------------------------------------------------------------
// - Definido  : conectar al servidor DNS principal.
//
// - Comentado : conectar al servidor DNS alternativo.
//
// Nota: se puede asignar el valor "gateway" (sin comillas) para usar la puerta
// de enlace como servidor DNS.
//
// Advertencia: si se usa un servidor DNS externo, este no podrá resolver
// hostnames de dispositivos pertenecientes a la red local.
// ------------------------------------------------------------------------------
// #define   USE_MAIN_DNS_SERVER

// ------------------------------------------------------------------------------
// Dirección IPv4 del servidor DNS principal.
//
// Valor predeterminado :                                                 gateway
// ------------------------------------------------------------------------------
#define   MAIN_DNS_SERVER                                                 gateway

// ------------------------------------------------------------------------------
// Dirección IPv4 del servidor DNS alternativo.
//
// Valor predeterminado :                                               "1.1.1.1"
// ------------------------------------------------------------------------------
#define   ALT_DNS_SERVER                                                "8.8.8.8"

// ------------------------------------ NTP -------------------------------------

// ------------------------------------------------------------------------------
// - Definido  : conectar al servidor NTP principal.
//
// - Comentado : conectar al servidor NTP alternativo.
// ------------------------------------------------------------------------------
// #define   USE_MAIN_NTP_SERVER

// ------------------------------------------------------------------------------
// Hostname del servidor NTP principal.
//
// Valor predeterminado :                                    "MY-MAIN-NTP-SERVER"
// ------------------------------------------------------------------------------
#define   MAIN_NTP_SERVER                                              "JORGE-PC"

// ------------------------------------------------------------------------------
// Hostname del servidor NTP alternativo.
//
// Valor predeterminado :                                       "es.pool.ntp.org"
// ------------------------------------------------------------------------------
#define   ALT_NTP_SERVER                                       "time.windows.com"

// ------------------------------------------------------------------------------
// Zona horaria GMT.
//
// Valor predeterminado :                                                       0
// ------------------------------------------------------------------------------
#define   GMT_TIME                                                             +1

// ------------------------------------ TLS -------------------------------------

// ------------------------------------------------------------------------------
// - Definido  : usar una conexión TLS con autenticación del servidor.
//
// - Comentado : usar una conexión insegura.
// ------------------------------------------------------------------------------
#define   USE_TLS

// ------------------------------------------------------------------------------
// - Definido  : incorporar el certificado del cliente y conectar a un broker que
//               lo requiera.
//
// - Comentado : NO incorporar el certificado del cliente y conectar a un broker
//               que NO lo requiera.
//
// Nota: esta definición no tiene efecto en conexiones inseguras.
// ------------------------------------------------------------------------------
// #define   USE_CLIENT_AUTH

// ------------------------------------------------------------------------------
// Nivel de depuración por printf() de mbedTLS, desde 0 (desactivado) hasta 5
// (depuración máxima). No se consideran los mensajes principales del cliente TLS
// (esos salen siempre).
//
// Nota: es útil para obtener las claves intercambiadas durante el handshake TLS,
// con las que se podrá desencriptar la sesión TLS con un sniffer de red como
// Wireshark (siendo necesario como mínimo un nivel 3 de depuración).
//
// Valor predeterminado :                                                       0
// ------------------------------------------------------------------------------
#define   TLS_DEBUG_LEVEL                                                       0

// ------------------------------------ MQTT ------------------------------------

// ------------------------------------------------------------------------------
// - Definido  : conectar al broker MQTT principal.
//
// - Comentado : conectar al broker MQTT alternativo.
// ------------------------------------------------------------------------------
// #define   USE_MAIN_MQTT_BROKER

// ------------------------------------------------------------------------------
// Hostname del broker MQTT principal.
//
// Valor predeterminado :                                   "MY-MAIN-MQTT-BROKER"
// ------------------------------------------------------------------------------
#define   MAIN_MQTT_BROKER                                             "JORGE-PC"

// ------------------------------------------------------------------------------
// Hostname del broker MQTT alternativo.
//
// Valor predeterminado :                                    "test.mosquitto.org"
// ------------------------------------------------------------------------------
#define   ALT_MQTT_BROKER                                    "test.mosquitto.org"

// ------------------------------------------------------------------------------
// Identificador del cliente MQTT.
//
// Valor predeterminado :                                        "[MY_CLIENT_ID]"
// ------------------------------------------------------------------------------
#define   CLIENT_ID                                                     "[JORGE]"

// ------------------------------------------------------------------------------
// Segundos de Keep Alive.
//
// Nota: para que el broker no desconecte al cliente por inactividad, en esta
// aplicación el Keep Alive debe ser bastante superior a 10 segundos, a menos que
// se desactive asignándole el valor 0. Sin embargo, esto último podría dar
// problemas en algunos brokers.
//
// Valor predeterminado :                                                       0
// ------------------------------------------------------------------------------
#define   KEEP_ALIVE                                                           60

// ------------------------------------------------------------------------------
// Tema público MQTT.
//
// Valor predeterminado :                                     "[MY_PUBLIC_TOPIC]"
// ------------------------------------------------------------------------------
#define   PUBLIC_TOPIC                                           "[TEMA_PÚBLICO]"

// ------------------------------------------------------------------------------
// Tema secreto MQTT.
//
// Valor predeterminado :                                     "[MY_SECRET_TOPIC]"
// ------------------------------------------------------------------------------
#define   SECRET_TOPIC                                           "[TEMA_SECRETO]"

// ------------------------------------------------------------------------------
// - Definido  : habilitar el modo de pruebas.
//
// - Comentado : deshabilitar el modo de pruebas.
// ------------------------------------------------------------------------------
// #define   TEST_MODE

// ------------------------------------------------------------------------------
// Tamaño de los mensajes MQTT enviados y recibidos en el modo de pruebas. Ha de
// ser bastante inferior al tamaño del buffer de recepción (2048 bytes) para
// poder recibir los mismos mensajes que se envían, ya que el tamaño de un
// paquete MQTT es superior al tamaño del mensaje que contiene, debido a que hay
// que sumarle el resto de bytes que forman dicho paquete. Y hay que tener en
// cuenta que, si se usa una conexión segura, un paquete MQTT pasará a ser un
// paquete TLS de mayor tamaño.
//
// Advertencia: si el tamaño de los paquetes supera el tamaño máximo de segmento
// TCP (1460 bytes), estos serán divididos, lo que va a influir en los resultados
// de la prueba.
//
// Valor predeterminado :                                                    1024
// ------------------------------------------------------------------------------
#define   TEST_MSG_BYTES                                                     1024

// ------------------------------------------------------------------------------
// Número de ciclos que se realizan en el modo de pruebas.
//
// Nota: 1 ciclo = 1 envío + 1 recepción.
//
// Advertencia: un número reducido de ciclos hará que los resultados de la prueba
// no sean fiables.
//
// Valor predeterminado :                                                     100
// ------------------------------------------------------------------------------
#define   TEST_N_CYCLES                                                       100

// ------------------------------------------------------------------------------

#endif // SETTINGS_H
