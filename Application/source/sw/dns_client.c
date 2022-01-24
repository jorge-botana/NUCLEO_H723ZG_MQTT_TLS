// ------------------------------------------------------------------------------
// DNS_CLIENT
//
// - Cliente DNS IPv4.
//
// - Requiere de UDP_CLIENT.
//
// - No emplea librerías DNS de terceros, y supone una alternativa a funciones
//   como gethostbyname() o getaddrinfo().
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


#include "dns_client.h"

#include "udp_client.h"

#include <stdio.h>
#include <stdlib.h>


#define   DNS_PORT                                                             53
#define   HOST_START                                                           12
#define   DNS_PACKET_MIN_SIZE                                                  17
#define   DNS_PACKET_MAX_SIZE                                                 512


int DNS_Request(const char *server, const char *hostname, size_t len, char *ipaddr)
{
	printf(" ---------- Cliente DNS -----------\n\n");

	// Omitir la resolución DNS en caso de que se haya introducido una dirección
	// IPv4 por hostname. Se presupone en un principio que esto es así, y se
	// comprueba caracter por caracter en busca de caracteres distintos a los que
	// aparecerían en una dirección IPv4.
	int hostname_is_ipaddr = 1;
	int i = 0;
	for(i = 0; i < len; i++)
	{
		if(hostname[i] != '0' && hostname[i] != '1' && \
		   hostname[i] != '2' && hostname[i] != '3' && \
		   hostname[i] != '4' && hostname[i] != '5' && \
		   hostname[i] != '6' && hostname[i] != '7' && \
		   hostname[i] != '8' && hostname[i] != '9' && \
		   hostname[i] != '.' && hostname[i] != 0)
		{
			hostname_is_ipaddr = 0;
		}
	}

	if(hostname_is_ipaddr == 0)
	{
		// Calcular la longitud total de este paquete DNS.
		size_t dns_request_len = DNS_PACKET_MIN_SIZE + len;

		// Alocar e inicializar a cero memoria para el paquete DNS.
		char *DNS_REQUEST = calloc(dns_request_len, sizeof(char));

		// - Transaction ID
		DNS_REQUEST[ 0] = rand();
		DNS_REQUEST[ 1] = rand();

		// - Flags
		DNS_REQUEST[ 2] = 0b00000001;
		                //  7||||||| - Response: Message is a query
		                //   6543||| - Opcode: Standard query (0)
		                //       2||
		                //        1| - Truncated: Message is not truncated
		                //         0 - Recursion desired: Do query recursively

		DNS_REQUEST[ 3] = 0b00000000;
		                //  7|||||||
		                //   6|||||| - Z: reserved (0)
		                //    5|||||
		                //     4|||| - Non-authenticated data: Unacceptable
		                //      3210

		// - Questions      : 1
		DNS_REQUEST[ 4] = 0x00;
		DNS_REQUEST[ 5] = 0x01;

		// - Answer RRs     : 0
		DNS_REQUEST[ 6] = 0x00;
		DNS_REQUEST[ 7] = 0x00;

		// - Authority RRs  : 0
		DNS_REQUEST[ 8] = 0x00;
		DNS_REQUEST[ 9] = 0x00;

		// - Additional RRs : 0
		DNS_REQUEST[10] = 0x00;
		DNS_REQUEST[11] = 0x00;

		// - Queries

		int i = 0;          // Posición actual en la cadena de texto del
		                    // hostname.

		int j = 0;          // Contador de caracteres en una palabra (p.e. "time"
		                    // o "com").

		int k = HOST_START; // Posición a partir de la cual se comienza a
		                    // escribir el hostname en el paquete.

		// -> Name

		// Mientras no se haya llegado al final del hostname ...
		do
		{
			// ... se reinicia el número de caracteres hasta el siguiente punto o
			// hasta el final del hostname.
			j = 0;

			// Mientras no se haya llegado al siguiente punto o al final del
			// hostname ...
			while((hostname[i + j] != '.') && \
			      (hostname[i + j] != 0  ))
			{
				// ... se calcula el número de caracteres hasta el siguiente
				// punto o hasta el final del hostname.
				j++;
			}

			// Se copia al paquete el número de caracteres hasta el siguiente
			// punto o hasta el final del hostname.
			DNS_REQUEST[k] = j;

			// Se determina dónde dejar de escribir, cuando se haya llegado al
			// siguiente punto o al final del hostname.
			j = j + k;
			for(k = k + 1; k <= j; k++)
			{
				// Caracter actual del hostname copiado al paquete.
				DNS_REQUEST[k] = hostname[i];

				// Se avanza al siguiente caracter del hostname.
				i++;
			}

			// Se salta el punto para la siguiente iteración (los puntos no se
			// copian al paquete). Si se tratase del NULL final, también hay que
			// incrementar i para indicar que se ha llegado al final.
			i++;

			// Si se ha llegado al final, añadir el NULL final antes de salir del
			// bucle.
			if(i == len)
			{
				DNS_REQUEST[k] = 0;
			}
		}
		while(i < len);

		// -> Type: A (Host Address) (1)
		DNS_REQUEST[k + 1] = 0;
		DNS_REQUEST[k + 2] = 1;

		// -> Class: IN
		DNS_REQUEST[k + 3] = 0;
		DNS_REQUEST[k + 4] = 1;

		// Realizar la comunicación y luego liberar la memoria alocada para el
		// paquete DNS enviado.
		char recv_buf[DNS_PACKET_MAX_SIZE] = {0};
		int ret = UDP_Request(server, server, DNS_PORT, DNS_REQUEST, dns_request_len, recv_buf, sizeof(recv_buf));
		free(DNS_REQUEST);
		if(ret < 0)
		{
			printf(" - No se pudo resolver %s\n\n", hostname);
			return ret;
		}

		// Obtener la última dirección IP de la respuesta DNS (se suelen recibir
		// varias, pero resulta sencillo coger la última porque son los últimos 4
		// bytes del paquete recibido).
		char get_IP[4] = {0};
		for(i = 0; i < 4; i++)
		{
			get_IP[i] = recv_buf[ret - 4 + i];
		}
		sprintf(ipaddr, "%i.%i.%i.%i", get_IP[0], get_IP[1], get_IP[2], get_IP[3]);
	}
	else
	{
		// Si el hostname introducido es la dirección IP, entonces copiar el
		// hostname a ipaddr, sin hacer la resolución DNS.
		for(i = 0; i < len; i++)
		{
			ipaddr[i] = hostname[i];
		}
		printf(" - DNS omitido\n");
	}

	// Mostrar la dirección IP del hostname tras haberla guardado en un buffer.
	printf(" -> Hostname    : %s\n"  , hostname);
	printf(" -> IP address  : %s\n\n", ipaddr  );

	return 0;
}
