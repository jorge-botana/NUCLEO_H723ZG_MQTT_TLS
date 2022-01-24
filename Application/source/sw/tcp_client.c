// ------------------------------------------------------------------------------
// TCP_CLIENT
//
// - Cliente TCP sobre IPv4.
//
// - Requiere de una implementación externa de la API de sockets BSD, que ha de
//   definirse en un símbolo de la configuración del proyecto:
//
//   -> Symbol : SOCKETS_IMPLEMENTATION
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


#include "tcp_client.h"

#ifndef   SOCKETS_IMPLEMENTATION
#error   "SOCKETS_IMPLEMENTATION                   no definido en el proyecto!!!"
#else  // SOCKETS_IMPLEMENTATION
#include  SOCKETS_IMPLEMENTATION

#include <stdio.h>


int s = 0;


static void Error_Code(int ret);


int TCP_Init(const char *hostname, const char *ipaddr, const int port)
{
	printf(" - Conectando a %s:%i ...", hostname, port);
	fflush(stdout);

	// Crear el socket TCP.
	s = socket(AF_INET, SOCK_STREAM, 0);

	// Establecer un timeout de recepción de 10 segundos.
	struct timeval timeout = {0};
	timeout.tv_sec         = 10;
	timeout.tv_usec        =  0;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

	// Establecer la dirección IP y el puerto del servidor.
	struct sockaddr_in dst = {0};
	dst.sin_len            = sizeof(dst);
	dst.sin_family         = AF_INET;
	dst.sin_port           = htons(port);
	dst.sin_addr.s_addr    = inet_addr(ipaddr);

	// Conectarse al servidor.
	int ret = connect(s, (struct sockaddr *)&dst, dst.sin_len);
	Error_Code(ret);

	return ret;
}


int TCP_Close(char *buf, size_t len)
{
	printf(" - Cerrando conexión ...........");
	fflush(stdout);

	// Procesar los datos aún no leídos para cerrar la conexión correctamente.
	while(read(s, buf, len) > 0) {};

	// Desconectarse del servidor y cerrar el socket.
	int ret = close(s);
	Error_Code(ret);

	return ret;
}


int TCP_Read(char *buf, size_t len)
{
	// Leer, bloqueando hasta que sea lea un dato o hasta el timeout.
	return read(s, buf, len);
}


int TCP_Write(const char *buf, size_t len)
{
	// Escribir, bloqueando hasta que se termine de escribir.
	return write(s, buf, len);
}


static void Error_Code(int ret)
{
	// Indicar si la conexión o desconexión se ha realizado correctamente.
	if(ret < 0)
	{
		printf(" error !!!\n");
	}
	else
	{
		printf(" ok\n");
	}
	printf("\n");
}


#endif // SOCKETS_IMPLEMENTATION
