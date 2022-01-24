// ------------------------------------------------------------------------------
// TLS_CLIENT
//
// - Cliente TLS 1.2.
//
// - Comprueba si el certificado del servidor es válido y, en caso de que falle
//   la verificación, el handshake TLS es abortado al no poder autenticarse el
//   servidor.
//
// - Permite añadir su propio certificado y clave privada, por si el servidor
//   requiriese de la autenticación del cliente.
//
// - Requiere de:
//
//   -> Un archivo de configuración.
//   -> Un archivo del que cargar certificados y claves privadas.
//   -> Una implementación externa de un RNG.
//   -> Una implementación externa de un RTC.
//   -> TCP_CLIENT.
//   -> El stack de TLS mbedTLS.
//
// - Algunas dependencias han de definirse en símbolos de la configuración del
//   proyecto:
//
//   -> Symbol : X_FILE_OR_IMPLEMENTATION
//   -> Value  : filename.h
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


#include "tls_client.h"

#ifndef   TLS_CLIENT_CONFIG_FILE
#error   "TLS_CLIENT_CONFIG_FILE                  no definido en el proyecto !!!"
#else  // TLS_CLIENT_CONFIG_FILE
#include  TLS_CLIENT_CONFIG_FILE
// Comprobar:
// - Si está definido   USE_CLIENT_AUTH
// - Valor de           TLS_DEBUG_LEVEL
#endif // TLS_CLIENT_CONFIG_FILE

#ifndef   CERTIFICATES_FILE
#error   "CERTIFICATES_FILE                       no definido en el proyecto !!!"
#else  // CERTIFICATES_FILE
#include  CERTIFICATES_FILE
// Definir y establecer el certificado del CA de confianza usado para autenticar
// el servidor:
// - CA_CRT
// Si se habilita la autenticación del cliente, también se ha de definir y
// establecer su propio certificado y clave privada:
// - CLIENT_CRT
// - CLIENT_KEY
#endif // CERTIFICATES_FILE

#ifndef   RNG_IMPLEMENTATION
#error   "RNG_IMPLEMENTATION                      no definido en el proyecto !!!"
#else  // RNG_IMPLEMENTATION
#include  RNG_IMPLEMENTATION
// Función requerida: unsigned int RNG_Get(void);
#endif // RNG_IMPLEMENTATION

#ifndef   RTC_IMPLEMENTATION
#error   "RTC_IMPLEMENTATION                      no definido en el proyecto !!!"
#else  // RTC_IMPLEMENTATION
#include  RTC_IMPLEMENTATION
// Función requerida: time_t RTC_Read(void);
#endif // RTC_IMPLEMENTATION

#ifndef   MBEDTLS_CONFIG_FILE
#error   "MBEDTLS_CONFIG_FILE                     no definido en el proyecto !!!"
#else  // MBEDTLS_CONFIG_FILE
// Fichero personalizado de configuración de mbedTLS que se adjunta con este
// cliente. Aunque no sea una dependencia directa del cliente, igualmente es
// requerido por él.
#endif // MBEDTLS_CONFIG_FILE

#include "tcp_client.h"

#include "mbedtls/ctr_drbg.h"
#if       TLS_DEBUG_LEVEL > 0
#include "mbedtls/debug.h"
#endif // TLS_DEBUG_LEVEL > 0
#include "mbedtls/entropy.h"
#include "mbedtls/error.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/version.h"

#include <stdio.h>
#include <string.h>


mbedtls_ssl_context      tls      = {0};
mbedtls_ssl_config       conf     = {0};
mbedtls_x509_crt         ca_crt   = {0};
#ifdef    USE_CLIENT_AUTH
mbedtls_x509_crt         cli_crt  = {0};
mbedtls_pk_context       cli_key  = {0};
#endif // USE_CLIENT_AUTH
mbedtls_ctr_drbg_context ctr_drbg = {0};
mbedtls_entropy_context  entropy  = {0};


static mbedtls_time_t RTC_Time(mbedtls_time_t *time);
static int CRT_Verify_Result(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags);
static void CRT_Info(mbedtls_x509_crt *crt);
#if       TLS_DEBUG_LEVEL > 0
static void mbedTLS_Debug(void *ctx, int level, const char *file, int line, const char *str);
#endif // TLS_DEBUG_LEVEL > 0
static int Error_Code(int ret);


int TLS_Init(const char *hostname, const char *ipaddr, const int port)
{
	// Establecer una conexión TCP.
	int ret = -1;
	if((ret = TCP_Init(hostname, ipaddr, port)) < 0)
	{
		return ret;
	}

	printf(" ---------- Cliente TLS -----------\n\n");

	// Mostrar la versión de mbedTLS.
	printf(" - %s\n\n", MBEDTLS_VERSION_STRING_FULL);

	// Inicializar las variables de mbedTLS.
	printf(" - Inicializando los datos de la sesión TLS ......");
	fflush(stdout);
	mbedtls_ssl_init       (&tls     );
	mbedtls_ssl_config_init(&conf    );
	mbedtls_x509_crt_init  (&ca_crt  );
#ifdef    USE_CLIENT_AUTH
	mbedtls_x509_crt_init  (&cli_crt );
	mbedtls_pk_init        (&cli_key );
#endif // USE_CLIENT_AUTH
	mbedtls_ctr_drbg_init  (&ctr_drbg);
	mbedtls_entropy_init   (&entropy );
	printf(" ok\n");

	// Establecer el callback de lectura del RTC.
	mbedtls_platform_set_time(RTC_Time);

#if       TLS_DEBUG_LEVEL > 0
	// Establecer el nivel de depuración de mbedTLS.
	mbedtls_debug_set_threshold(TLS_DEBUG_LEVEL);
#endif // TLS_DEBUG_LEVEL > 0

	// Generar la semilla aleatoria con el RNG.
	printf(" - Generando la semilla aleatoria ................");
	fflush(stdout);
	if((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0)) < 0)
	{
		printf(" error !!!\n\n");
		printf(" - mbedtls_ctr_drbg_seed() returned -0x%04X\n", -ret);
		return Error_Code(ret);
	}
	printf(" ok\n");

	// Cargar el certificado del CA de confianza usado para autenticar el
	// servidor. Normalmente es un certificado raíz.
	printf(" - Cargando el certificado del CA de confianza ...");
	fflush(stdout);
	if((ret = mbedtls_x509_crt_parse(&ca_crt , (const unsigned char *)CA_CRT, sizeof(CA_CRT))) < 0)
	{
		printf(" error !!!\n\n");
		printf(" - mbedtls_x509_crt_parse() returned -0x%04X\n", -ret);
		return Error_Code(ret);
	}
	printf(" ok\n");

#ifdef    USE_CLIENT_AUTH
	// Cargar el certificado del cliente.
	printf(" - Cargando el certificado del cliente ...........");
	fflush(stdout);
	if((ret = mbedtls_x509_crt_parse(&cli_crt, (const unsigned char *)CLIENT_CRT, sizeof(CLIENT_CRT))) < 0)
	{
		printf(" error !!!\n\n");
		printf(" - mbedtls_x509_crt_parse() returned -0x%04X\n", -ret);
		return Error_Code(ret);
	}
	printf(" ok\n");

	// Cargar la clave privada del cliente.
	printf(" - Cargando la clave privada del cliente .........");
	fflush(stdout);
	if((ret = mbedtls_pk_parse_key(&cli_key, (const unsigned char *)CLIENT_KEY, sizeof(CLIENT_KEY), NULL, 0)) < 0)
	{
		printf(" error !!!\n\n");
		printf(" - mbedtls_pk_parse_key() returned -0x%04X\n", -ret);
		return Error_Code(ret);
	}
	printf(" ok\n");
#endif // USE_CLIENT_AUTH

	// Comenzar a crear la estructura TLS.
	printf(" - Creando la estructura TLS .....................");
	fflush( stdout );

	// Inicializar una configuración TLS predeterminada para un cliente TLS.
	if((ret = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) < 0)
	{
		printf(" error !!!\n\n");
		printf(" - mbedtls_ssl_config_defaults() returned -0x%04X\n", -ret);
		return Error_Code(ret);
	}

	// En la configuración TLS, establecer el callback que muestra durante el
	// handshake TLS el resultado de la verificación de la cadena de confianza
	// del servidor.
	mbedtls_ssl_conf_verify(&conf, CRT_Verify_Result, NULL);

	// En la configuración TLS, requerir de la autenticación del servidor para
	// que el handshake TLS sea satisfactorio.
	mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_REQUIRED);

	// En la configuración TLS, establecer el callback de generación de números
	// aleatorios a partir de la semilla aleatoria ya creada.
	mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);

#if       TLS_DEBUG_LEVEL > 0
	// En la configuración TLS, establecer el callback de depuración de mbedTLS.
	mbedtls_ssl_conf_dbg(&conf, mbedTLS_Debug, NULL);
#endif // TLS_DEBUG_LEVEL > 0

	// En la configuración TLS, establecer el certificado del CA de confianza
	// usado para autenticar el servidor.
	mbedtls_ssl_conf_ca_chain(&conf, &ca_crt, NULL);

#ifdef    USE_CLIENT_AUTH
	// En la configuración TLS, asociar el certificado del cliente con su clave
	// privada.
	if((ret = mbedtls_ssl_conf_own_cert(&conf, &cli_crt, &cli_key)) < 0)
	{
		printf(" error !!!\n\n");
		printf(" - mbedtls_ssl_conf_own_cert() returned -0x%04X\n", -ret);
		return Error_Code(ret);
	}
#endif // USE_CLIENT_AUTH

	// En el contexto TLS, cargar la configuración TLS.
	if((ret = mbedtls_ssl_setup(&tls, &conf)) < 0)
	{
		printf(" error !!!\n\n");
		printf(" - mbedtls_ssl_setup() returned -0x%04X\n", -ret);
		return Error_Code(ret);
	}

	// En el contexto TLS, establecer el hostname del servidor. Debe aparecer en
	// su certificado.
	if((ret = mbedtls_ssl_set_hostname(&tls, hostname)) < 0)
	{
		printf(" error !!!\n\n");
		printf(" - mbedtls_ssl_set_hostname() returned -0x%04X\n", -ret);
		return Error_Code(ret);
	}

	// En el contexto TLS, establecer el socket TCP y los callbacks de envío y
	// recepción.
	mbedtls_ssl_set_bio(&tls, &s, mbedtls_net_send, mbedtls_net_recv, NULL);

	// Terminar de crear la estructura TLS.
	printf(" ok\n");

	// Inicializar el handshake TLS.
	printf(" - Realizando el handshake TLS ......................\n\n");
	if((ret = mbedtls_ssl_handshake(&tls)) < 0)
	{
		printf(" - mbedtls_ssl_handshake() returned -0x%04X\n", -ret);
		return Error_Code(ret);
	}

#ifdef    USE_CLIENT_AUTH
	// Mostrar el certificado del cliente.
	printf(" - Certificado del cliente\n");
	CRT_Info(&cli_crt);
	printf(" - Ok: validado por el servidor\n\n");
#endif // USE_CLIENT_AUTH

	// Salir tras completar el handshake TLS.
	printf(" - Handshake TLS satisfactorio\n\n");

	return 0;
}


int TLS_Close(char *buf, size_t len)
{
	// Liberar los recursos empleados por mbedTLS.
	mbedtls_ssl_free       (&tls     );
	mbedtls_ssl_config_free(&conf    );
	mbedtls_x509_crt_free  (&ca_crt  );
#ifdef    USE_CLIENT_AUTH
	mbedtls_x509_crt_free  (&cli_crt );
	mbedtls_pk_free        (&cli_key );
#endif // USE_CLIENT_AUTH
	mbedtls_ctr_drbg_free  (&ctr_drbg);
	mbedtls_entropy_free   (&entropy );

	// Realizar el handshake de desconexión TCP.
	return TCP_Close(buf, len);
}


int TLS_Read(char *buf, size_t len)
{
	// Leer datos ya desencriptados por mbedTLS.
	return mbedtls_ssl_read(&tls, (unsigned char *)buf, len);
}


int TLS_Write(const char *buf, size_t len)
{
	// Escribir datos que serán encriptados por mbedTLS.
	return mbedtls_ssl_write(&tls, (const unsigned char *)buf, len);
}


int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
	// Evitar "warnings" en el compilador debido a argumentos sin usar.
	(void)data;

	// Generar la semilla aleatoria con el RNG. Se generan y guardan 4 bytes por
	// iteración, hasta completar la semilla.
	int i = 0;
	for(i = 0; i < len / 4; i++)
	{
		unsigned int random = RNG_Get();
		*olen = *olen + 4;
		memset(&(output[4 * i]), (int)random, 4);
	}

	return 0;
}


int mbedtls_net_recv(void *ctx, unsigned char *buf, size_t len)
{
	// Evitar "warnings" en el compilador debido a argumentos sin usar.
	(void)ctx;

	// Leer datos encriptados.
	return TCP_Read((char *)buf, len);
}


int mbedtls_net_send(void *ctx, const unsigned char *buf, size_t len)
{
	// Evitar "warnings" en el compilador debido a argumentos sin usar.
	(void)ctx;

	// Escribir datos encriptados.
	return TCP_Write((const char *)buf, len);
}


static mbedtls_time_t RTC_Time(mbedtls_time_t *time)
{
	// Evitar "warnings" en el compilador debido a argumentos sin usar.
	(void)time;

	// Para verificar que funciona la comprobación de la fecha y hora de emisión
	// y expiración de los certificados, o para establecerla manualmente, se
	// puede sustituir RTC_Read() por el timestamp correspondiente a una fecha y
	// hora concreta (GMT 0).

	// return 1577836800; // Comienzo del año 2020.

	// return 1735689600; // Comienzo del año 2025.

	// return 2208988800; // Comienzo del año 2040.

	// Obtener el timestamp.
	return RTC_Read();
}


static int CRT_Verify_Result(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags)
{
	// Evitar "warnings" en el compilador debido a argumentos sin usar.
	(void)data;
	(void)depth;
	(void)flags;

	// Construir la cadena de confianza del servidor. Antes del handshake, el
	// certificado del CA de confianza es añadido con mbedtls_x509_crt_parse() y
	// es establecido como certificado de confianza con
	// mbedtls_ssl_conf_ca_chain().

	// Este certificado del CA de confianza normalmente es un certificado raíz.
	// El resto de certificados de la cadena de confianza deberían ser
	// proporcionados por el servidor durante el handshake TLS. Es posible que el
	// servidor también proporcione el certificado raíz, aunque lo mismo da
	// porque, para que sea de confianza para el cliente, este ya debe contar con
	// él antes del handshake TLS.

	// Tras construir la cadena de confianza del servidor, la verificación se
	// realiza desde el certificado del servidor hasta el certificado del CA de
	// confianza. Luego se muestra la cadena de confianza del servidor con el
	// resultado de la verificación.

	// Nota: mbedTLS permite instalar un certificado de un CA intermedio como
	// certificado de confianza sin el certificado raíz. De todas formas, esto no
	// es muy lógico y en otras implementaciones de TLS podría no estar
	// permitido, al intentarse seguir la cadena hacia el certificado raíz.

	printf(" - Cadena de confianza del servidor (depth = %i)\n", depth);

	// Mostrar la información del certificado.
	CRT_Info(crt);

	// Mostrar el resultado de la verificación.
	if(*flags == 0)
	{
		printf(" - Ok: verificación correcta\n\n");
	}
	else
	{
		char buf[2048] = {0};
		mbedtls_x509_crt_verify_info(buf, sizeof(buf), " - Error: ", *flags);
		printf("%s\n", buf);
		fflush(stdout);
	}

	return 0;
}


static void CRT_Info(mbedtls_x509_crt *crt)
{
	// Mostrar la información del certificado.
	char buf[2048] = {0};
	mbedtls_x509_crt_info(buf, sizeof(buf), " -> ", crt);
	printf("%s", buf);
}


#if       TLS_DEBUG_LEVEL > 0

static void mbedTLS_Debug(void *ctx, int level, const char *file, int line, const char *str)
{
	// Evitar "warnings" en el compilador debido a argumentos sin usar.
	(void)(ctx  );
	(void)(level);

	// Mostrar el mensaje de depuración (archivo, línea, mensaje).
	printf(" - File: %s\n", file);
	printf(" - Line: %i\n", line);
	printf(" - Msg : %s\n", str );
}

#endif // TLS_DEBUG_LEVEL > 0


static int Error_Code(int ret)
{
	// Mostrar el mensaje de error correspondiente a un código de error.
	char buf[2048] = {0};
	mbedtls_strerror(ret, buf, sizeof(buf));
	printf(" -> %s\n\n", buf);

	return ret;
}
