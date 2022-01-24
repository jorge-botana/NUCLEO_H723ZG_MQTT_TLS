// ------------------------------------------------------------------------------
// TCPIP_SETUP
//
// - Inicialización del stack de TCP/IP lwIP.
//
// - Activación y desactivación de la interfaz de red.
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


#include "tcpip_setup.h"

#include "lwip/init.h"
#include "lwip/tcpip.h"


struct netif netif = {0};


void TCPIP_Init(void)
{
	printf(" - Inicializando TCP/IP ........");
	fflush(stdout);

	// Crear la tarea de TCP/IP.
	tcpip_init(NULL, NULL);

	// Inicializar la interfaz de red.
	ip4_addr_t ipaddr  = {0};
	ip4_addr_t netmask = {0};
	ip4_addr_t gw      = {0};
	netif_add        (&netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
	netif_set_default(&netif);
	netif_set_link_up(&netif);
	netif_set_up     (&netif);

	printf(" ok\n");

	// Mostrar la versión de lwIP.
	printf(" -> lwIP %i.%i.%i\n",
		LWIP_VERSION_MAJOR,
		LWIP_VERSION_MINOR,
		LWIP_VERSION_REVISION);

	// Mostrar la dirección MAC.
	printf(" -> MAC address : %02X:%02X:%02X:%02X:%02X:%02X\n\n", \
		netif.hwaddr[0],                                          \
		netif.hwaddr[1],                                          \
		netif.hwaddr[2],                                          \
		netif.hwaddr[3],                                          \
		netif.hwaddr[4],                                          \
		netif.hwaddr[5]);
}


void TCPIP_Down_Up(void)
{
#ifdef    STM32H723xx
	// "Workaround" para hacer frente a un problema por el cual solo funciona el
	// primer socket creado: tras cerrarlo y abrir un segundo socket, este último
	// no funcionará a menos que se añadan las dos siguientes líneas antes de
	// crear cada socket:

	netif_set_down(&netif);
	netif_set_up  (&netif);

	// El problema aparece cuando se habilita la D-Cache + MPU en un STM32H723.
#endif // STM32H723xx
}
