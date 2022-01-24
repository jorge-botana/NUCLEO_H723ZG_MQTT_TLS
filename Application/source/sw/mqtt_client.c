// ------------------------------------------------------------------------------
// MQTT_CLIENT
//
// - Cliente MQTT v5.0 (QoS 0).
//
// - Permite conexiones con y sin TLS.
//
// - Paquetes MQTT implementados:
//
//   -> 00 - Reserved   - No          -> 08 - SUBSCRIBE   - Sí ( W)
//   -> 01 - CONNECT    - Sí ( W)     -> 09 - SUBACK      - Sí (R )
//   -> 02 - CONNACK    - Sí (R )     -> 10 - UNSUBSCRIBE - Sí ( W)
//   -> 03 - PUBLISH    - Sí (RW)     -> 11 - UNSUBACK    - Sí (R )
//   -> 04 - PUBACK     - No          -> 12 - PINGREQ     - Sí ( W)
//   -> 05 - PUBREC     - No          -> 13 - PINGRESP    - Sí (R )
//   -> 06 - PUBREL     - No          -> 14 - DISCONNECT  - Sí ( W)
//   -> 07 - PUBCOMP    - No          -> 15 - AUTH        - No
//
// - Requiere de:
//
//   -> Un archivo de configuración.
//   -> Una implementación externa de un temporizador para hacer tests midiendo
//      tiempos de escritura y lectura de paquetes MQTT. Si no se van a hacer
//      estos tests, entonces no es necesario el temporizador.
//   -> TLS_CLIENT (si se usa una conexión TLS).
//   -> TCP_CLIENT (si se usa una conexión insegura).
//
// - Algunas dependencias han de definirse en símbolos de la configuración del
//   proyecto:
//
//   -> Symbol : X_FILE_OR_IMPLEMENTATION
//   -> Value  : filename.h
//
// - No emplea librerías MQTT de terceros.
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


#include "mqtt_client.h"

#ifndef   MQTT_CLIENT_CONFIG_FILE
#error   "MQTT_CLIENT_CONFIG_FILE                 no definido en el proyecto !!!"
#else  // MQTT_CLIENT_CONFIG_FILE
#include  MQTT_CLIENT_CONFIG_FILE
// Comprobar:
// - Si está definido   TEST_MODE
// - Si está definido   USE_TLS
#endif // MQTT_CLIENT_CONFIG_FILE

#ifdef    TEST_MODE
#ifndef   TIM_IMPLEMENTATION
#error   "TIM_IMPLEMENTATION                      no definido en el proyecto !!!"
#else  // TIM_IMPLEMENTATION
#include  TIM_IMPLEMENTATION
// Funciones requeridas: void TIM_Start(void);
//                       void TIM_Stop(void);
#endif // TIM_IMPLEMENTATION
#endif // TEST_MODE

#ifdef    USE_TLS
#include "tls_client.h"
#else  // USE_TLS
#include "tcp_client.h"
#endif // USE_TLS

#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define   FIXED_HEADER_MIN_SIZE                                                 2

#define   CONNECT_MIN_SIZE                                                     15
#define   DISCONNECT_MIN_SIZE                                                   2
#define   PUBLISH_MIN_SIZE                                                      5
#define   SUBSCRIBE_MIN_SIZE                                                    8
#define   UNSUBSCRIBE_MIN_SIZE                                                  7
#define   PINGREQ_MIN_SIZE                                                      2


static int MQTT_Read_CONNACK(char *buf, size_t ret);
static int MQTT_Read_PUBLISH(char *buf, size_t ret);
static int MQTT_Read_SUBACK(char *buf, size_t ret);
static int MQTT_Read_UNSUBACK(char *buf, size_t ret);
static int MQTT_Read_PINGRESP(char *buf, size_t ret);

static int MQTT_Write_Packet(const char *buf, size_t len);

static int MQTT_Generate_Packet_ID(void);


int MQTT_Init(const char *hostname, const char *ipaddr, const int port)
{
	printf(" ---------- Cliente MQTT ----------\n\n");

	// Realizar el handshake de conexión.
#ifdef    USE_TLS
	return TLS_Init(hostname, ipaddr, port);
#else  // USE_TLS
	return TCP_Init(hostname, ipaddr, port);
#endif // USE_TLS
}


int MQTT_Close(char *buf, size_t len)
{
	// Realizar el handshake de desconexión.
#ifdef    USE_TLS
	return TLS_Close(buf, len);
#else  // USE_TLS
	return TCP_Close(buf, len);
#endif // USE_TLS
}


int MQTT_Read_Packet(char *buf, size_t len)
{
	printf(" - Esperando paquetes .............\n");

#ifdef    TEST_MODE
	// Iniciar el temporizador.
	TIM_Start();
#endif // TEST_MODE

	// Leer paquetes entrantes.
#ifdef    USE_TLS
	int ret = TLS_Read(buf, len);
#else
	int ret = TCP_Read(buf, len);
#endif // USE_TLS

#ifdef    TEST_MODE
	// Detener el temporizador.
	TIM_Stop();
#endif // TEST_MODE

	// Procesar el paquete leído, si lo hay.
	if(ret < 0)
	{
		printf(" - Ningún paquete recibido\n");
	}
	else
	{
		// Determinar el tipo de paquete leído, que viene definido por el nibble
		// superior del primer byte. A continuación, llamar a la función
		// correspondiente.

		char packet_type = buf[0] & 0b11110000;

		printf(" - Recibido ");

		if     (packet_type == 0b00100000)
		{
			printf("CONNACK\n" );
			ret = MQTT_Read_CONNACK (buf, ret);
		}
		else if(packet_type == 0b00110000)
		{
			printf("PUBLISH\n" );
			ret = MQTT_Read_PUBLISH (buf, ret);
		}
		else if(packet_type == 0b10010000)
		{
			printf("SUBACK\n"  );
			ret = MQTT_Read_SUBACK  (buf, ret);
		}
		else if(packet_type == 0b10110000)
		{
			printf("UNSUBACK\n");
			ret = MQTT_Read_UNSUBACK(buf, ret);
		}
		else if(packet_type == 0b11010000)
		{
			printf("PINGRESP\n");
			ret = MQTT_Read_PINGRESP(buf, ret);
		}
		else
		{
			printf("paquete desconocido !!!\n\n");
			ret = -1;
		}
	}

	return ret;
}


static int MQTT_Read_CONNACK(char *buf, size_t ret)
{
	// Leer el "Reason Code". Es el cuarto byte del paquete CONNACK. Si vale 0,
	// es que la conexión ha sido aceptada.
	if(buf[3] == 0)
	{
		printf(" - Conexión aceptada\n\n");

		return ret;
	}
	else
	{
		printf(" - Error de conexión !!!\n\n");

		return -1;
	}
}


static int MQTT_Read_PUBLISH(char *buf, size_t ret)
{
	// Calcular la longitud del mensaje recibido. Explicación:

	// - ret es el tamaño total del paquete PUBLISH.
	size_t m_len = ret;

	// - Restar 1 byte del campo "Header Flags".
	m_len--;

	// - Restar n bytes del campo "Remaining Length". Deja de haber bytes cuando
	//   se encuentra un 1 en el MSB de un byte.
	int n = 0;
	do
	{
		n++;
	}
	while((buf[n] & 0b10000000) != 0);
	m_len = m_len - n;

	// - Restar 2 bytes del campo "Topic Length".
	m_len = m_len - 2;

	// - Restar el número de bytes del tema.
	size_t t_len = 128 * buf[n + 1] + buf[n + 2];
	m_len = m_len - t_len;

	// - Restar 1 byte del campo "Properties" -> "Total Length".
	m_len = m_len - 1;

	// - Restar p bytes de propiedades.
	size_t p = buf[n + 2 + t_len + 1];
	m_len = m_len - p;

	// Desplazar el mensaje (contenido al final del paquete) al comienzo del
	// buffer.
	int i = 0;
	for(i = 0; i < m_len; i++)
	{
		buf[i] = buf[i + (ret - m_len)];
	}

	// Marcar el final del mensaje recibido con un NULL.
	buf[i] = 0;

	// Mostrar el mensaje recibido.
	printf(" <- %s\n\n", buf);

	return ret;
}


static int MQTT_Read_SUBACK(char *buf, size_t ret)
{
	// Comprobar si la suscripción ha sido aceptada. Para ello hay que examinar
	// el último byte recibido del paquete SUBACK (para "ret" bytes recibidos, se
	// construye el buffer desde la posición 0 hasta la posición ret - 1).
	if(buf[ret - 1] == 0)
	{
		printf(" - Suscripción aceptada : QoS 0\n\n");

		return ret;
	}
	else
	{
		printf(" - Error de conexión !!!\n\n");

		return -1;
	}
}


static int MQTT_Read_UNSUBACK(char *buf, size_t ret)
{
	// Comprobar si la desuscripción ha sido aceptada. Para ello hay que examinar
	// el último byte recibido del paquete UNSUBACK (para "ret" bytes recibidos,
	// se construye el buffer desde la posición 0 hasta la posición ret - 1).
	if(buf[ret - 1] == 0)
	{
		printf(" - Desuscripción aceptada\n\n");

		return ret;
	}
	else
	{
		printf(" - Error de conexión !!!\n\n");

		return -1;
	}
}


static int MQTT_Read_PINGRESP(char *buf, size_t ret)
{
	// Evitar "warnings" en el compilador debido a argumentos sin usar.
	(void)buf;

	printf(" - La conexión sigue activa\n\n");

	return ret;
}


int MQTT_Write_CONNECT(const char *client, size_t c_len, const int keep_alive)
{
	printf(" - Enviando CONNECT ............");
	fflush(stdout);

	// Descontar el NULL de la longitud.
	c_len--;

	// Calcular el número de bytes necesarios para codificar "Remaining Length".
	size_t n     = 0;
	size_t power = 0;
	do
	{
		n++;
		power = pow(128, n);
	}
	while((CONNECT_MIN_SIZE - FIXED_HEADER_MIN_SIZE + c_len) / power > 0);

	// Calcular la longitud total de este paquete CONNECT.
	size_t connect_len = CONNECT_MIN_SIZE + (n - 1) + c_len;

	// Alocar memoria para el paquete CONNECT.
	char *CONNECT = calloc(connect_len, sizeof(char));

	// - Header Flags
	CONNECT[0] = 0b00010000;
	           //  7654|||| - Message Type: Connect Command (1)
	           //      3210 - Reserved: 0

	// - Remaining Length
	size_t rem_len = connect_len - FIXED_HEADER_MIN_SIZE - (n - 1);
	int i = 0;
	do
	{
		CONNECT[1 + i] = rem_len % 128;
		rem_len        = rem_len / 128;
		if(rem_len > 0)
		{
			CONNECT[1 + i] = CONNECT[1 + i] | 128;
			i++;
		}
	}
	while(rem_len > 0);

	// - Protocol Name Length
	CONNECT[ 2 + i] = 0; // MSB
	CONNECT[ 3 + i] = 4; // LSB

	// - Protocol Name
	CONNECT[ 4 + i] = 'M';
	CONNECT[ 5 + i] = 'Q';
	CONNECT[ 6 + i] = 'T';
	CONNECT[ 7 + i] = 'T';

	// - Version
	CONNECT[ 8 + i] = 5;

	// - Connect Flags
	CONNECT[ 9 + i] = 0b00000010;
	                //  7||||||| - User Name Flag: Not set
	                //   6|||||| - Password Flag: Not set
	                //    5||||| - Will Retain: Not set
	                //     43||| - QoS Level: At most once delivery (Fire and
	                //       |||   Forget) (0)
	                //       2|| - Will Flag: Not set
	                //        1| - Clean Session Flag: Set
	                //         0 - (Reserved): Not set

	// - Keep Alive
	CONNECT[10 + i] = keep_alive / 256; // MSB
	CONNECT[11 + i] = keep_alive % 256; // LSB

	// - Properties
	CONNECT[12 + i] = 0; // - Total Length
		// La longitud es 0 porque no hay propiedades. En caso de haber
		// propiedades, estas irían a continuación.

	// - Client ID Length
	CONNECT[13 + i] = c_len / 256; // MSB
	CONNECT[14 + i] = c_len % 256; // LSB

	// - Client ID
	int j = 0;
	for(j = 0; j < c_len; j++)
	{
		CONNECT[15 + i + j] = client[j];
	}

	// Enviar el paquete CONNECT y liberar la memoria alocada.
	int ret = MQTT_Write_Packet(CONNECT, connect_len);
	free(CONNECT);

	return ret;
}


int MQTT_Write_PUBLISH(const char *topic, size_t t_len, const char *msg, size_t m_len)
{
	printf(" - Enviando PUBLISH ............");
	fflush(stdout);

	// Descontar el NULL de las longitudes.
	t_len--;
	m_len--;

	// Calcular el número de bytes necesarios para codificar "Remaining Length".
	size_t n     = 0;
	size_t power = 0;
	do
	{
		n++;
		power = pow(128, n);
	}
	while((PUBLISH_MIN_SIZE - FIXED_HEADER_MIN_SIZE + t_len + m_len) / power > 0);

	// Calcular la longitud total de este paquete PUBLISH.
	size_t publish_len = PUBLISH_MIN_SIZE + (n - 1) + t_len + m_len;

	// Alocar memoria para el paquete PUBLISH.
	char *PUBLISH = calloc(publish_len, sizeof(char));

	// - Header Flags
	PUBLISH[0] = 0b00110000;
	           //  7654|||| - Message Type: Publish Command (3)
	           //      3||| - DUP Flag: Not set
	           //       21| - QoS Level: At most once delivery (Fire and Forget)
	           //         |   (0)
	           //         0 - Retain: Not set

	// - Remaining Length
	size_t rem_len = publish_len - FIXED_HEADER_MIN_SIZE - (n - 1);
	int i = 0;
	do
	{
		PUBLISH[1 + i] = rem_len % 128;
		rem_len        = rem_len / 128;
		if(rem_len > 0)
		{
			PUBLISH[1 + i] = PUBLISH[1 + i] | 128;
			i++;
		}
	}
	while(rem_len > 0);

	// - Topic Length
	PUBLISH[2 + i] = t_len / 256; // MSB
	PUBLISH[3 + i] = t_len % 256; // LSB

	// - Topic
	int j = 0;
	for(j = 0; j < t_len; j++)
	{
		PUBLISH[4 + i + j] = topic[j];
	}

	// - Properties
	PUBLISH[4 + i + j] = 0; // - Total Length
		// La longitud es 0 porque no hay propiedades. En caso de haber
		// propiedades, estas irían a continuación.

	// - Message
	int k = 0;
	for(k = 0; k < m_len; k++)
	{
		PUBLISH[5 + i + j + k] = msg[k];
	}

	// Enviar el paquete PUBLISH y liberar la memoria alocada.
	int ret = MQTT_Write_Packet(PUBLISH, publish_len);
	free(PUBLISH);

	// Mostrar el mensaje enviado.
	printf(" -> %s\n\n", msg);

	return ret;
}


int MQTT_Write_SUBSCRIBE(const char *topic, size_t t_len)
{
	printf(" - Enviando SUBSCRIBE ..........");
	fflush(stdout);

	// Descontar el NULL de la longitud.
	t_len--;

	// Calcular el número de bytes necesarios para codificar "Remaining Length".
	size_t n     = 0;
	size_t power = 0;
	do
	{
		n++;
		power = pow(128, n);
	}
	while((SUBSCRIBE_MIN_SIZE - FIXED_HEADER_MIN_SIZE + t_len) / power > 0);

	// Calcular la longitud total de este paquete SUBSCRIBE.
	size_t subscribe_len = SUBSCRIBE_MIN_SIZE + (n - 1) + t_len;

	// Alocar memoria para el paquete SUBSCRIBE.
	char *SUBSCRIBE = calloc(subscribe_len, sizeof(char));

	// - Header Flags
	SUBSCRIBE[0] = 0b10000010;
	             //  7654|||| - Message Type: Subscribe Request (8)
	             //      3210 - Reserved: 2

	// - Remaining Length
	size_t rem_len = subscribe_len - FIXED_HEADER_MIN_SIZE - (n - 1);
	int i = 0;
	do
	{
		SUBSCRIBE[1 + i] = rem_len % 128;
		rem_len          = rem_len / 128;
		if(rem_len > 0)
		{
			SUBSCRIBE[1 + i] = SUBSCRIBE[1 + i] | 128;
			i++;
		}
	}
	while(rem_len > 0);

	// - Message Identifier
	int packet_identifier = MQTT_Generate_Packet_ID();
	SUBSCRIBE[2 + i] = packet_identifier / 128; // MSB
	SUBSCRIBE[3 + i] = packet_identifier % 128; // LSB

	// - Properties
	SUBSCRIBE[4 + i] = 0; // - Total Length
		// La longitud es 0 porque no hay propiedades. En caso de haber
		// propiedades, estas irían a continuación.

	// - Topic Length
	SUBSCRIBE[5 + i] = t_len / 256; // MSB
	SUBSCRIBE[6 + i] = t_len % 256; // LSB

	// - Topic
	int j = 0;
	for(j = 0; j < t_len; j++)
	{
		SUBSCRIBE[7 + i + j] = topic[j];
	}

	// - Subscription Options
	SUBSCRIBE[7 + i + j] = 0b00000000;
	                     //  76|||||| - Reserved: 0x0
	                     //    54|||| - Retain Handling: Send msgs at
	                     //      ||||   subscription time (0)
	                     //      3||| - Retain as Published: Not set
	                     //       2|| - No Local: Not set
	                     //        10 - QoS: At most once delivery (Fire and
	                     //             Forget) (0)

	// Enviar el paquete SUBSCRIBE y liberar la memoria alocada.
	int ret = MQTT_Write_Packet(SUBSCRIBE, subscribe_len);
	free(SUBSCRIBE);

	return ret;
}


int MQTT_Write_UNSUBSCRIBE(const char *topic, size_t t_len)
{
	printf(" - Enviando UNSUBSCRIBE ........");
	fflush(stdout);

	// Descontar el NULL de la longitud.
	t_len--;

	// Calcular el número de bytes necesarios para codificar "Remaining Length".
	size_t n     = 0;
	size_t power = 0;
	do
	{
		n++;
		power = pow(128, n);
	}
	while((UNSUBSCRIBE_MIN_SIZE - FIXED_HEADER_MIN_SIZE + t_len) / power > 0);

	// Calcular la longitud total de este paquete UNSUBSCRIBE.
	size_t unsubscribe_len = UNSUBSCRIBE_MIN_SIZE + (n - 1) + t_len;

	// Alocar memoria para el paquete UNSUBSCRIBE.
	char *UNSUBSCRIBE = calloc(unsubscribe_len, sizeof(char));

	// - Header Flags
	UNSUBSCRIBE[0] = 0b10100010;
	               //  7654|||| - Message Type: Unsubscribe Request (10)
	               //      3210 - Reserved: 2

	// - Remaining Length
	size_t rem_len = unsubscribe_len - FIXED_HEADER_MIN_SIZE - (n - 1);
	int i = 0;
	do
	{
		UNSUBSCRIBE[1 + i] = rem_len % 128;
		rem_len            = rem_len / 128;
		if(rem_len > 0)
		{
			UNSUBSCRIBE[1 + i] = UNSUBSCRIBE[1 + i] | 128;
			i++;
		}
	}
	while(rem_len > 0);

	// - Message Identifier
	int packet_identifier = MQTT_Generate_Packet_ID();
	UNSUBSCRIBE[2 + i] = packet_identifier / 128; // MSB
	UNSUBSCRIBE[3 + i] = packet_identifier % 128; // LSB

	// - Properties
	UNSUBSCRIBE[4 + i] = 0; // - Total Length
		// La longitud es 0 porque no hay propiedades. En caso de haber
		// propiedades, estas irían a continuación.

	// - Topic Length
	UNSUBSCRIBE[5 + i] = t_len / 256; // MSB
	UNSUBSCRIBE[6 + i] = t_len % 256; // LSB

	// - Topic
	int j = 0;
	for(j = 0; j < t_len; j++)
	{
		UNSUBSCRIBE[7 + i + j] = topic[j];
	}

	// Enviar el paquete UNSUBSCRIBE y liberar la memoria alocada.
	int ret = MQTT_Write_Packet(UNSUBSCRIBE, unsubscribe_len);
	free(UNSUBSCRIBE);

	return ret;
}


int MQTT_Write_PINGREQ(void)
{
	printf(" - Enviando PINGREQ ............");
	fflush(stdout);

	// Inicializar memoria para el paquete PINGREQ.
	char PINGREQ[PINGREQ_MIN_SIZE] = {0};

	// - Header Flags
	PINGREQ[0] = 0b11000000;
	           //  7654|||| - Message Type: Ping Request (12)
	           //      3210 - Reserved: 0

	// - Remaining Length
	PINGREQ[1] = 0;

	// Enviar el paquete PINGREQ.
	return MQTT_Write_Packet(PINGREQ, PINGREQ_MIN_SIZE);
}


int MQTT_Write_DISCONNECT(void)
{
	printf(" - Enviando DISCONNECT .........");
	fflush(stdout);

	// Inicializar memoria para el paquete DISCONNECT.
	char DISCONNECT[DISCONNECT_MIN_SIZE] = {0};

	// - Header Flags
	DISCONNECT[0] = 0b11100000;
	              //  7654|||| - Message Type: Disconnect Req (14)
	              //      3210 - Reserved: 0

	// - Remaining Length
	DISCONNECT[1] = 0;

	// Enviar el paquete DISCONNECT.
	return MQTT_Write_Packet(DISCONNECT, DISCONNECT_MIN_SIZE);
}


static int MQTT_Write_Packet(const char *buf, size_t len)
{
#ifdef    TEST_MODE
	// Iniciar el temporizador.
	TIM_Start();
#endif // TEST_MODE

	// Escribir el paquete.
#ifdef    USE_TLS
	int ret = TLS_Write(buf, len);
#else  // USE_TLS
	int ret = TCP_Write(buf, len);
#endif // USE_TLS

#ifdef    TEST_MODE
	// Detener el temporizador.
	TIM_Stop();
#endif // TEST_MODE

	// Indicar si el paquete se ha enviado correctamente.
	if(ret < 0)
	{
		printf(" error !!!\n");
	}
	else
	{
		printf(" ok\n");
	}

	return ret;
}


static int MQTT_Generate_Packet_ID(void)
{
	// Generar un nuevo identificador del paquete.
	static int packet_identifier = 0;
	if(++packet_identifier == 0)
	{
		// El 0 está prohibido por el protocolo, por lo que si hay un overflow
		// del 65535 al 0, hay que saltar al 1.
		packet_identifier++;
	}

	// Devolver el identificador del paquete generado.
	return packet_identifier;
}
