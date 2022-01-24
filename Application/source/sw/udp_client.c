// ------------------------------------------------------------------------------
// UDP_CLIENT
//
// - Cliente UDP sobre IPv4.
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


#include "udp_client.h"

#ifndef   SOCKETS_IMPLEMENTATION
#error   "SOCKETS_IMPLEMENTATION                   no definido en el proyecto!!!"
#else  // SOCKETS_IMPLEMENTATION
#include  SOCKETS_IMPLEMENTATION


static void Error_Code(int ret);


int UDP_Request(const char *hostname, const char *ipaddr, const int port, const char *s_buf, size_t s_len, char *r_buf, size_t r_len)
{
	printf(" - Preguntando a %s:%i ...", hostname, port);
	fflush(stdout);

	// Crear el socket UDP.
	int s = socket(AF_INET, SOCK_DGRAM, 0);

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

	// Enviar el segmento UDP al servidor.
	sendto(s, s_buf, s_len, 0, (struct sockaddr *)&dst, dst.sin_len);

	// Obtener y guardar en un buffer la respuesta del servidor.
	struct sockaddr src     = {0};
	socklen_t       src_len = sizeof(src);
	int ret = recvfrom(s, r_buf, r_len, 0, &src, &src_len);
	Error_Code(ret);

	// Cerrar el socket.
	close(s);

	return ret;
}


static void Error_Code(int ret)
{
	// Indicar si se ha obtenido respuesta del servidor.
	if(ret < 0)
	{
		printf(" error !!!\n");
	}
	else
	{
		printf(" ok\n");
	}
}


#endif // SOCKETS_IMPLEMENTATION
