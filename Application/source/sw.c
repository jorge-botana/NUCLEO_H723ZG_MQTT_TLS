// ------------------------------------------------------------------------------
// SW
//
// - API que contiene funciones que hacen uso de TCP/IP.
//
// - Las cadenas de texto han de incluir el NULL final.
//
// - Los tamaños han de contar el NULL final.
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


#include "sw.h"

#include "tcpip_setup.h"
#include "dhcp_client.h"
#include "dns_client.h"
#include "ntp_client.h"
#include "mqtt_client.h"


// ------------------------------------------------------------------------------
// Inicializa el stack de TCP/IP.
//
// -> (void    ) Nada.
//
// <- (void    ) Nada.
// ------------------------------------------------------------------------------
void Sw_TCPIP_Init(void)
{
	TCPIP_Init();
}


// ------------------------------------------------------------------------------
// Establece la configuración de red por medio de un cliente DHCP. Se debe
// ejecutar periódicamente hasta que esto ocurra.
//
// -> (int     ) Número máximo de intentos del cliente DHCP hasta establecer una
//               configuración de red estática.
//
// -> (char   *) Dirección IPv4 estática usada en caso de que NO se obtenga la
//               configuración de red por DHCP.
//
// -> (char   *) Máscara de subred estática usada en caso de que NO se obtenga la
//               configuración de red por DHCP.
//
// -> (char   *) Puerta de enlace estática usada en caso de que NO se obtenga la
//               configuración de red por DHCP.
//
// -> (char   *) Buffer donde se guardará la puerta de enlace.
//
// <- (int     ) 0 en caso de que se haya establecido la configuración de red (ya
//               sea por DHCP o de forma estática tras agotar el número de
//               intentos), un valor negativo en caso de que aún se esté en
//               proceso de establecer la configuración de red.
// ------------------------------------------------------------------------------
int Sw_DHCP_Request(const int dhcp_attempts, const char *static_ip, const char *static_netmask, const char *static_gw, char *gateway)
{
	return DHCP_Request(dhcp_attempts, static_ip, static_netmask, static_gw, gateway);
}


// ------------------------------------------------------------------------------
// Obtiene la dirección IPv4 correspondiente a un hostname mediante una
// resolución DNS.
//
// -> (char   *) Dirección IPv4 del servidor DNS.
//
// -> (char   *) Hostname que se quiere resolver.
//
// -> (size_t  ) Tamaño del hostname.
//
// -> (char   *) Buffer donde se guardará la dirección IPv4 del hostname.
//
// <- (int     ) 0 si NO hay error, un valor negativo en caso contrario.
// ------------------------------------------------------------------------------
int Sw_DNS_Request(const char *server, const char *hostname, size_t len, char *ipaddr)
{
	TCPIP_Down_Up();

	return DNS_Request(server, hostname, len, ipaddr);
}


// ------------------------------------------------------------------------------
// Obtiene la fecha y hora conectando a un servidor NTP.
//
// -> (char   *) Hostname del servidor NTP.
//
// -> (char   *) Dirección IPv4 del servidor NTP.
//
// -> (int     ) Zona horaria GMT.
//
// -> (time_t *) Buffer donde se guardará el timestamp.
//
// <- (int     ) 0 si NO hay error, un valor negativo en caso contrario.
// ------------------------------------------------------------------------------
int Sw_NTP_Request(const char *hostname, const char *ipaddr, const int gmt_time, time_t *curtime)
{
	TCPIP_Down_Up();

	return NTP_Request(hostname, ipaddr, gmt_time, curtime);
}


// ------------------------------------------------------------------------------
// Conecta a un broker MQTT.
//
// -> (char   *) Hostname del broker MQTT.
//
// -> (char   *) Dirección IPv4 del broker MQTT.
//
// -> (int     ) Puerto usado por el broker MQTT.
//
// <- (int     ) 0 si NO hay error, un valor negativo en caso contrario.
// ------------------------------------------------------------------------------
int Sw_MQTT_Init(const char *hostname, const char *ipaddr, const int port)
{
	TCPIP_Down_Up();

	return MQTT_Init(hostname, ipaddr, port);
}


// ------------------------------------------------------------------------------
// Desconecta de un broker MQTT.
//
// -> (char   *) Buffer de recepción.
//
// -> (size_t  ) Tamaño del buffer de recepción.
//
// <- (int     ) 0 si NO hay error, un valor negativo en caso contrario.
// ------------------------------------------------------------------------------
int Sw_MQTT_Close(char *buf, size_t len)
{
	return MQTT_Close(buf, len);
}


// ------------------------------------------------------------------------------
// Espera a paquetes MQTT entrantes. Bloquea hasta que se recibe un paquete o
// hasta el timeout. Procesa el paquete recibido, si lo hay.
//
// -> (char   *) Buffer de recepción.
//
// -> (size_t  ) Tamaño del buffer de recepción.
//
// <- (int     ) Número de bytes leídos o, en caso de error o timeout, un valor
//               negativo.
// ------------------------------------------------------------------------------
int Sw_MQTT_Read_Packet(char *buf, size_t len)
{
	return MQTT_Read_Packet(buf, len);
}


// ------------------------------------------------------------------------------
// Envía un paquete CONNECT con el Client ID y los segundos de Keep Alive.
//
// -> (char   *) Client ID.
//
// -> (size_t  ) Tamaño del Client ID.
//
// -> (int     ) Segundos de Keep Alive.
//
// <- (int     ) Número de bytes escritos o, en caso de error, un valor negativo.
// ------------------------------------------------------------------------------
int Sw_MQTT_Write_CONNECT(const char *client, size_t c_len, const int keep_alive)
{
	return MQTT_Write_CONNECT(client, c_len, keep_alive);
}


// ------------------------------------------------------------------------------
// Envía un paquete DISCONNECT.
//
// -> (void    ) Nada.
//
// <- (int     ) Número de bytes escritos o, en caso de error, un valor negativo.
// ------------------------------------------------------------------------------
int Sw_MQTT_Write_DISCONNECT(void)
{
	return MQTT_Write_DISCONNECT();
}


// ------------------------------------------------------------------------------
// Envía un paquete PUBLISH con un tema y un mensaje.
//
// -> (char   *) Tema.
//
// -> (size_t  ) Tamaño del tema.
//
// -> (char   *) Mensaje.
//
// -> (size_t  ) Tamaño del mensaje.
//
// <- (int     ) Número de bytes escritos o, en caso de error, un valor negativo.
// ------------------------------------------------------------------------------
int Sw_MQTT_Write_PUBLISH(const char *topic, size_t t_len, const char *msg, size_t m_len)
{
	return MQTT_Write_PUBLISH(topic, t_len, msg, m_len);
}


// ------------------------------------------------------------------------------
// Envía un paquete SUBSCRIBE con un tema.
//
// -> (char   *) Tema.
//
// -> (size_t  ) Tamaño del tema.
//
// <- (int     ) Número de bytes escritos o, en caso de error, un valor negativo.
// ------------------------------------------------------------------------------
int Sw_MQTT_Write_SUBSCRIBE(const char *topic, size_t t_len)
{
	return MQTT_Write_SUBSCRIBE(topic, t_len);
}


// ------------------------------------------------------------------------------
// Envía un paquete UNSUBSCRIBE con un tema.
//
// -> (char   *) Tema.
//
// -> (size_t  ) Tamaño del tema.
//
// <- (int     ) Número de bytes escritos o, en caso de error, un valor negativo.
// ------------------------------------------------------------------------------
int Sw_MQTT_Write_UNSUBSCRIBE(const char *topic, size_t t_len)
{
	return MQTT_Write_UNSUBSCRIBE(topic, t_len);
}


// ------------------------------------------------------------------------------
// Envía un paquete PINGREQ.
//
// -> (void    ) Nada.
//
// <- (int     ) Número de bytes escritos o, en caso de error, un valor negativo.
// ------------------------------------------------------------------------------
int Sw_MQTT_Write_PINGREQ(void)
{
	return MQTT_Write_PINGREQ();
}
