// ------------------------------------------------------------------------------
// DHCP_CLIENT
//
// - Cliente DHCP IPv4.
//
// - Requiere del módulo de DHCP del stack de TCP/IP lwIP.
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


#include "dhcp_client.h"

#include "tcpip_setup.h"

#include "lwip/dhcp.h"
#include "lwip/inet.h"


#define   DHCP_START                                                           -2
#define   DHCP_WAIT                                                            -1
#define   DHCP_FINISH                                                           0


static void Netif_Info(struct netif netif, char *gateway);
static void Addr_Info(int addr);


int DHCP_Request(const int dhcp_attempts, const char *static_ip, const char *static_netmask, const char *static_gw, char *gateway)
{
	static int DHCP_state = DHCP_START;
	static int      count =          0;

	switch(DHCP_state)
	{
		// Inicializar el módulo de DHCP.
		case DHCP_START:
			printf(" ---------- Cliente DHCP ----------\n\n");
			if(dhcp_attempts > 0)
			{
				printf(" - Buscando servidor DHCP ");
				fflush(stdout);
				dhcp_start(&netif);
			}
			else
			{
				printf(" - DHCP omitido\n");
			}
			DHCP_state = DHCP_WAIT;
			break;

		// Esperar a que se obtenga la configuración de red.
		case DHCP_WAIT:
			if(dhcp_supplied_address(&netif))
			{
				// Usar la configuración de red obtenida por DHCP.
				printf(" ok\n");
				printf(" - Configuración de red asignada por un servidor DHCP\n");
				Netif_Info(netif, gateway);
				DHCP_state = DHCP_FINISH;
			}
			else
			{
				if(count < dhcp_attempts)
				{
					// Si el número de intentos es inferior al máximo permitido,
					// esperar a la vez que se van escribiendo puntos
					// suspensivos.
					count++;
					printf(".");
					fflush(stdout);
					DHCP_state = DHCP_WAIT;
				}
				else
				{
					// Parar el módulo de DHCP.
					if(dhcp_attempts > 0)
					{
						printf(" timeout !!!\n");
						dhcp_stop(&netif);
					}

					// Usar una configuración de red estática.
					const ip4_addr_t ip      = {
						.addr = inet_addr(static_ip     )
					};

					const ip4_addr_t netmask = {
						.addr = inet_addr(static_netmask)
					};

					const ip4_addr_t gw      = {
						.addr = inet_addr(static_gw     )
					};

					netif_set_addr(&netif, &ip, &netmask, &gw);

					printf(" - Configuración de red estática\n");
					Netif_Info(netif, gateway);
					DHCP_state = DHCP_FINISH;
				}
			}
			break;

		default:
			break;
	}

	return DHCP_state;
}


static void Netif_Info(struct netif netif, char *gateway)
{
	// Mostrar la configuración de red.
	printf(" -> IP address  : ");
	Addr_Info(netif.ip_addr.addr);
	printf(" -> Netmask     : ");
	Addr_Info(netif.netmask.addr);
	printf(" -> Gateway     : ");
	Addr_Info(netif.gw.addr     );
	printf("\n");

	// Guardar la puerta de enlace en un buffer.
	sprintf(gateway, "%s", inet_ntoa(netif.gw));
}


static void Addr_Info(int addr)
{
	// Mostrar una dirección IP, máscara de subred o puerta de enlace.
	printf("%i.%i.%i.%i\n",        \
		(addr & 0x000000FF) >>  0, \
		(addr & 0x0000FF00) >>  8, \
		(addr & 0x00FF0000) >> 16, \
		(addr & 0xFF000000) >> 24);
}
