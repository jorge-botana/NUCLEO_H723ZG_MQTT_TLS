// ------------------------------------------------------------------------------
// MAIN
//
// - Programa que implementa los siguientes protocolos TCP/IP:
//
//   -> DHCP                          -> TLS
//   -> DNS                           -> MQTT
//   -> NTP
//
// - Los protocolos anteriores, al formar parte de la capa de aplicación,
//   funcionan sobre la capa de transporte, a la cual pertenecen los siguientes
//   protocolos:
//
//   -> UDP                           -> TCP
//
// - Se han creado clientes para los siete protocolos anteriores.
//
// - Los clientes de la capa de transporte se han programado con la API de
//   sockets BSD. Para hacer uso de ella con el stack de TCP/IP empleado, es
//   necesario un RTOS. Se tienen las siguientes tareas, ordenadas de arriba a
//   abajo de mayor a menor prioridad:
//
//   -> Tarea de Ethernet (en el archivo de usuario "eth.c").
//   -> Tarea de TCP/IP (en el archivo de lwIP "tcpip.c").
//   -> Tarea que mira a ver si se ha perdido la conexión LAN.
//   -> Tarea que mira a ver si el botón ha sido pulsado.
//   -> Tarea principal del programa.
//
// - Se proporciona un archivo de configuración del programa. Sus opciones más
//   significativas son aquellas que hacen referencia a la seguridad de la
//   comunicación MQTT, pudiéndose elegir entre:
//
//   -> Conexión insegura.
//   -> Conexión TLS con autenticación del servidor.
//   -> Conexión TLS con autenticación del servidor y del cliente.
//
// - El funcionamiento del programa se detalla a lo largo del mismo.
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


#include "hw.h"
#include "sw.h"
#include "setup.h"

#include "cmsis_os.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define   MAIN_THREAD_STACK_SIZE                   (6 * configMINIMAL_STACK_SIZE)
#define   LINK_THREAD_STACK_SIZE                   (1 * configMINIMAL_STACK_SIZE)
#define   BUTTON_THREAD_STACK_SIZE                 (2 * configMINIMAL_STACK_SIZE)

#define   RECV_BUF_SIZE                                                      2048

#define   MSG_CONNECTED                                       "[PLACA CONECTADA]"
#define   MSG_DISCONNECTED                                 "[PLACA DESCONECTADA]"

#define   MSG_LD1_ON                                          "[LED 1 ENCENDIDO]"
#define   MSG_LD1_OFF                                           "[LED 1 APAGADO]"
#define   MSG_LD2_ON                                          "[LED 2 ENCENDIDO]"
#define   MSG_LD2_OFF                                           "[LED 2 APAGADO]"
#define   MSG_LD3_ON                                          "[LED 3 ENCENDIDO]"
#define   MSG_LD3_OFF                                           "[LED 3 APAGADO]"

#define   FIRST_PRINTABLE_CHAR                                                 32
#define   LAST_PRINTABLE_CHAR                                                 126


osThreadAttr_t thread_attr  = {0};
osThreadId_t   main_handler = {0};

char mqtt_recv_buf[RECV_BUF_SIZE] = {0};

#ifdef    TEST_MODE
char mqtt_test_msg[TEST_MSG_BYTES + 1] = {0};
#endif // TEST_MODE


static void Main_Thread(void *arg);
static void Link_Thread(void *arg);
static void Button_Thread(void *arg);

#ifdef    TEST_MODE
static void Test_Mode(void);
#endif // TEST_MODE

static void Close_Program(void);
static void Error_Handler(void);


int main(void)
{
	// Inicializar los periféricos.
	Hw_Init();

#ifdef    UART_DEBUG
	// Activar la UART.
	Hw_UART_Start();
#endif // UART_DEBUG

	// Mostrar el mensaje inicial.
	printf("\n\n\n ------- MQTT/TLS demo v1.0 -------\n\n");

	printf(" - Autor        : Jorge Botana\n");
	printf(" - Plataforma   : %s\n", PLATFORM);

	// Generar la semilla aleatoria para la función rand().
	unsigned int seed = Hw_RNG_Get();
	srand(seed);
	printf(" - Semilla      : %010u\n\n", seed);

	// Inicializar CMSIS-RTOS2, mostrando la versión de FreeRTOS.
	printf(" - Inicializando RTOS ..........");
	fflush(stdout);
	osKernelInitialize();
	printf(" ok\n");
	printf(" -> FreeRTOS %s\n", tskKERNEL_VERSION_NUMBER);
	printf(" -> CMSIS-RTOS2 wrapper\n\n");

	// Crear la tarea principal.
	thread_attr.stack_size = MAIN_THREAD_STACK_SIZE;
	thread_attr.priority   = osPriorityBelowNormal;
	main_handler = osThreadNew(Main_Thread, NULL, &thread_attr);

	// Inicializar el scheduler.
	osKernelStart();
}


static void Main_Thread(void *arg)
{
	// Evitar "warnings" en el compilador debido a argumentos sin usar.
	(void)arg;

	// Encender el LED 1 tras inicializar el scheduler.
	Hw_LD1_On();

	// Inicializar el stack de TCP/IP.
	Sw_TCPIP_Init();

	// Bloquear el programa hasta que se detecte la conexión LAN.
	printf(" - Esperando al cable LAN ......");
	fflush(stdout);
	while(Hw_ETH_Link_Status() < 0) {}
	printf(" ok\n\n");

	// Inicializar la tarea que comprueba periódicamente el estado de la conexión
	// LAN, para terminar el programa si esta se pierde.
	thread_attr.stack_size = LINK_THREAD_STACK_SIZE;
	thread_attr.priority   = osPriorityAboveNormal;
	osThreadNew(Link_Thread, NULL, &thread_attr);

	// Intentar obtener la configuración de red por DHCP. Si se agotan los
	// intentos, usar una configuración de red estática. La puerta de enlace será
	// guardada en un buffer de 16 bytes (12 dígitos + 3 puntos + 1 NULL final),
	// para poder usarla como servidor en las resoluciones DNS (si no se usa
	// otro).
	char gateway[16] = {0};
	while(Sw_DHCP_Request(DHCP_ATTEMPTS, STATIC_IP, STATIC_NETMASK, STATIC_GW, gateway) < 0)
	{
		// Llamar a la función periódicamente cada 250 ms hasta que se establezca
		// la configuración de red.
		Hw_Tick_Delay(250);
	}

	// Encender el LED 2 tras establecer la configuración de red.
	Hw_LD2_On();

	// Obtener la dirección IPv4 del servidor NTP, que será guardada en un buffer
	// de 16 bytes (12 dígitos + 3 puntos + 1 NULL final).
	char ntp_ipaddr[16] = {0};
	if(Sw_DNS_Request(DNS_SERVER, NTP_SERVER, sizeof(NTP_SERVER), ntp_ipaddr) < 0)
	{
		Error_Handler();
	}

	// Obtener la fecha y hora del servidor NTP, guardando el timestamp.
	time_t curtime = 0;
	if(Sw_NTP_Request(NTP_SERVER, ntp_ipaddr, GMT_TIME, &curtime) < 0)
	{
		Error_Handler();
	}

	// Escribir la fecha y hora en el RTC.
	Hw_RTC_Write(curtime);

	// Obtener la dirección IPv4 del broker MQTT, que será guardada en un buffer
	// de 16 bytes (12 dígitos + 3 puntos + 1 NULL final).
	char mqtt_ipaddr[16] = {0};
	if(Sw_DNS_Request(DNS_SERVER, MQTT_BROKER, sizeof(MQTT_BROKER), mqtt_ipaddr) < 0)
	{
		Error_Handler();
	}

	// Realizar el handshake de conexión TCP con el broker MQTT. Si la conexión
	// es segura, también se realiza el handshake TLS.
	if(Sw_MQTT_Init(MQTT_BROKER, mqtt_ipaddr, MQTT_PORT) < 0)
	{
		Error_Handler();
	}

	// Conectarse al broker MQTT con el Client ID y los segundos de Keep Alive.
	Sw_MQTT_Write_CONNECT(CLIENT_ID, sizeof(CLIENT_ID), KEEP_ALIVE);

	// Esperar a que el broker acepte la conexión.
	if(Sw_MQTT_Read_Packet(mqtt_recv_buf, sizeof(mqtt_recv_buf)) < 0)
	{
		Error_Handler();
	}

	// Suscribirse al tema secreto.
	Sw_MQTT_Write_SUBSCRIBE(SECRET_TOPIC, sizeof(SECRET_TOPIC));

	// Esperar a que el broker acepte la suscripción.
	if(Sw_MQTT_Read_Packet(mqtt_recv_buf, sizeof(mqtt_recv_buf)) < 0)
	{
		Error_Handler();
	}

	// Publicar en el tema público que la placa ha sido conectada.
	Sw_MQTT_Write_PUBLISH(PUBLIC_TOPIC, sizeof(PUBLIC_TOPIC), MSG_CONNECTED, sizeof(MSG_CONNECTED));

	// Encender el LED 3 tras finalizar la configuración inicial.
	Hw_LD3_On();

#ifdef    TEST_MODE
	// Si el botón está pulsado ...
	if(Hw_B1_Read() == 1)
	{
		// ... entrar en el modo de pruebas para medir tiempos de escritura y
		// lectura de paquetes.
		Test_Mode();
	}
#endif // TEST_MODE

	// Inicializar la tarea que lee periódicamente el pulsador, para desconectar
	// el cliente MQTT y terminar el programa cuando este sea pulsado.
	thread_attr.stack_size = BUTTON_THREAD_STACK_SIZE;
	thread_attr.priority   = osPriorityNormal;
	osThreadNew(Button_Thread, NULL, &thread_attr);

	// Lazo sin fin. El programa publicará mensajes en el tema público y recibirá
	// aquellos que otros clientes publiquen en el tema secreto. Usar una
	// conexión TLS puede evitar que se filtre el tema secreto.
	for( ;; )
	{
		// Esperar a paquetes entrantes. Si la operación de lectura devuelve un
		// valor negativo, puede ser que:
		// - No se ha recibido ningún paquete en 10 segundos.
		// - Se ha perdido la conexión.
		if(Sw_MQTT_Read_Packet(mqtt_recv_buf, sizeof(mqtt_recv_buf)) < 0)
		{
			// Enviar un PINGREQ al cual el broker tendrá que contestar con un
			// PINGRESP, en caso de que la conexión siga activa.
			Sw_MQTT_Write_PINGREQ();

			// Si la operación de lectura devuelve de nuevo un valor negativo ...
			if(Sw_MQTT_Read_Packet(mqtt_recv_buf, sizeof(mqtt_recv_buf)) < 0)
			{
				// ... se debe a que se ha perdido la conexión, y por ello se
				// finaliza el programa.
				printf("\n - Conexión perdida con el broker !!!\n\n");
				Error_Handler();
			}
		}

		// Comprobar si el paquete recibido contiene un comando secreto para
		// encender o apagar los LEDs, y actuar en consecuencia. Una conexión TLS
		// puede evitar que se filtren estos comandos secretos.
		if     (strcmp(mqtt_recv_buf, "LD1ON" ) == 0)
		{
			Hw_LD1_On ();
			Sw_MQTT_Write_PUBLISH(PUBLIC_TOPIC, sizeof(PUBLIC_TOPIC), MSG_LD1_ON , sizeof(MSG_LD1_ON ));
		}
		else if(strcmp(mqtt_recv_buf, "LD1OFF") == 0)
		{
			Hw_LD1_Off();
			Sw_MQTT_Write_PUBLISH(PUBLIC_TOPIC, sizeof(PUBLIC_TOPIC), MSG_LD1_OFF, sizeof(MSG_LD1_OFF));
		}
		else if(strcmp(mqtt_recv_buf, "LD2ON" ) == 0)
		{
			Hw_LD2_On ();
			Sw_MQTT_Write_PUBLISH(PUBLIC_TOPIC, sizeof(PUBLIC_TOPIC), MSG_LD2_ON , sizeof(MSG_LD2_ON ));
		}
		else if(strcmp(mqtt_recv_buf, "LD2OFF") == 0)
		{
			Hw_LD2_Off();
			Sw_MQTT_Write_PUBLISH(PUBLIC_TOPIC, sizeof(PUBLIC_TOPIC), MSG_LD2_OFF, sizeof(MSG_LD2_OFF));
		}
		else if(strcmp(mqtt_recv_buf, "LD3ON" ) == 0)
		{
			Hw_LD3_On ();
			Sw_MQTT_Write_PUBLISH(PUBLIC_TOPIC, sizeof(PUBLIC_TOPIC), MSG_LD3_ON , sizeof(MSG_LD3_ON ));
		}
		else if(strcmp(mqtt_recv_buf, "LD3OFF") == 0)
		{
			Hw_LD3_Off();
			Sw_MQTT_Write_PUBLISH(PUBLIC_TOPIC, sizeof(PUBLIC_TOPIC), MSG_LD3_OFF, sizeof(MSG_LD3_OFF));
		}
		else
		{
			// Leer la fecha y hora del RTC.
			curtime = Hw_RTC_Read();

			// Publicar en el tema público la fecha y hora si el paquete recibido
			// no contenía un comando secreto.
			char buf[30] = {0};
			Sw_MQTT_Write_PUBLISH(PUBLIC_TOPIC, sizeof(PUBLIC_TOPIC), buf, strftime(buf, sizeof(buf), "%a %d-%m-%Y - %H:%M:%S", localtime(&curtime)) + 1);
		}
	}
}


static void Link_Thread(void *arg)
{
	// Evitar "warnings" en el compilador debido a argumentos sin usar.
	(void)arg;

	for( ;; )
	{
		// Mirar a ver si se ha perdido la conexión LAN.
		if(Hw_ETH_Link_Status() < 0)
		{
#ifdef    UART_DEBUG
			// Reiniciar la UART para evitar que deje de funcionar si se hubiese
			// interrumpido un printf() al cambiar de tarea.
			Hw_UART_Stop ();
			Hw_UART_Start();
#endif // UART_DEBUG

			// Finalizar el programa porque se ha perdido la conexión LAN.
			printf("\n - Cable LAN desconectado !!!\n\n");
			Error_Handler();
		}

		// Suspender la tarea durante 100 ms.
		osDelay(100);
	}
}


static void Button_Thread(void *arg)
{
	// Evitar "warnings" en el compilador debido a argumentos sin usar.
	(void)arg;

	for( ;; )
	{
		// Mirar a ver si el botón ha sido pulsado.
		if(Hw_B1_Read() == 1)
		{
#ifdef    UART_DEBUG
			// Reiniciar la UART para evitar que deje de funcionar si se hubiese
			// interrumpido un printf() al cambiar de tarea.
			Hw_UART_Stop ();
			Hw_UART_Start();
#endif // UART_DEBUG

			// Terminar la tarea principal ya que, de no hacerlo, seguiría
			// ejecutándose cuando la tarea del botón se suspende durante una
			// operación de lectura en el proceso de desconexión TCP.
			osThreadTerminate(main_handler);

			// Desconectarse del broker y finalizar el programa.
			printf("\n - Botón pulsado !!!\n\n");
			Close_Program();
		}

		// Suspender la tarea durante 50 ms.
		osDelay(50);
	}
}


#ifdef    TEST_MODE

static void Test_Mode(void)
{
	// Modo de pruebas para medir tiempos de escritura y lectura de paquetes (con
	// o sin TLS). No se mide el tiempo invertido durante el transporte de estos,
	// ni lo que tarda el broker en responder.

	// Se emplea un temporizador: el cliente MQTT lo inicia y lo para cuando
	// corresponda, para ser leído desde esta función.

	// Para medir los tiempos de escritura y lectura, se publica un mensaje
	// aleatorio en el tema secreto, al cual se está suscrito, para recibir una
	// respuesta inmediata del broker con ese mismo mensaje.

	// Repetir el ciclo el número de veces establecido y obtener los tiempos
	// promedios.

	printf(" -------- Modo de pruebas ---------\n\n");

	// -> n = Número de mediciones
	int n       = 0;

	// -> i = Medida actual
	int i_write = 0;
	int i_read  = 0;

	// -> s = Valor acumulado
	int s_write = 0;
	int s_read  = 0;

	// -> x = Media aritmética = s / n
	int x_write = 0;
	int x_read  = 0;

	for( ;; )
	{
		// Incrementar el número de la medición actual.
		n++;

		// Generar un mensaje aleatorio con caracteres imprimibles del código
		// ASCII original (7 bits). El último byte ha de ser un NULL (que luego
		// el cliente MQTT eliminará).
		int i = 0;
		for(i = 0; i < (sizeof(mqtt_test_msg) - 1); i++)
		{
			do
			{
				mqtt_test_msg[i] = rand();
			}
			while((mqtt_test_msg[i] < FIRST_PRINTABLE_CHAR) || \
			      (mqtt_test_msg[i] >  LAST_PRINTABLE_CHAR));
		}
		mqtt_test_msg[i] = 0;

		// Medir el tiempo de escritura.
		Sw_MQTT_Write_PUBLISH(SECRET_TOPIC, sizeof(SECRET_TOPIC), mqtt_test_msg, sizeof(mqtt_test_msg));

		// Actualizar las estadísticas de escritura.
		i_write = Hw_TIM_Read();
		s_write = s_write + i_write;
		x_write = s_write / n;

		// Dar tiempo a que llegue la respuesta, ya que solo se está midiendo el
		// tiempo invertido en escribir y leer los paquetes, y no el tiempo
		// invertido durante el transporte de estos.
		Hw_Tick_Delay(1000);

		// Medir el tiempo de lectura. Si la operación de lectura devuelve un
		// valor negativo, es porque ha habido un error de conexión, ya que el
		// broker debe devolver el mensaje que fue publicado en el mismo tema al
		// que se está suscrito.
		if(Sw_MQTT_Read_Packet(mqtt_recv_buf, sizeof(mqtt_recv_buf)) < 0)
		{
			// Interrumpir la prueba y finalizar el programa, ya que se ha
			// perdido la conexión.
			printf("\n - Conexión perdida con el broker !!!\n\n");
			Error_Handler();
		}

		// Actualizar las estadísticas de lectura.
		i_read = Hw_TIM_Read();
		s_read = s_read + i_read;
		x_read = s_read / n;

		// Mostrar los tiempos de la medición actual.
		printf(" - Ciclo %i/%i\n", n, TEST_N_CYCLES);
		printf(" -> Escritura : %i us\n"  , i_write);
		printf(" -> Lectura   : %i us\n\n", i_read );

		// Mostrar los tiempos promedios.
		printf(" - Promedios\n");
		printf(" -> Escritura : %i us\n"  , x_write);
		printf(" -> Lectura   : %i us\n\n", x_read );

		// Realizar el ciclo el número de veces especificado y luego terminar la
		// prueba.
		if(n < TEST_N_CYCLES)
		{
			// Esperar un segundo antes del siguiente ciclo.
			Hw_Tick_Delay(1000);
		}
		else
		{
			// Mostrar los resultados de la prueba.
			printf(" -------- Prueba terminada --------\n\n");

#ifdef    USE_TLS
			printf(" - Conexión TLS\n"     );
#else  // USE_TLS
			printf(" - Conexión insegura\n");
#endif // USE_TLS
			printf(" -> Broker    : %s\n"  , MQTT_BROKER   );
			printf(" -> Puerto    : %i\n"  , MQTT_PORT     );
			printf(" -> bytes/msg : %i\n"  , TEST_MSG_BYTES);
			printf(" -> Ciclos    : %i\n\n", TEST_N_CYCLES );

			printf(" - Resultados\n");
			printf(" -> Escritura : %i us\n"  , x_write);
			printf(" -> Lectura   : %i us\n\n", x_read );

			// Desconectarse del broker y finalizar el programa.
			printf(" ----------------------------------\n\n");

			Close_Program();
		}
	}
}

#endif // TEST_MODE


static void Close_Program(void)
{
	// Desuscribirse del tema secreto.
	Sw_MQTT_Write_UNSUBSCRIBE(SECRET_TOPIC, sizeof(SECRET_TOPIC));

	// Esperar a que el broker acepte la desuscripción.
	if(Sw_MQTT_Read_Packet(mqtt_recv_buf, sizeof(mqtt_recv_buf)) < 0)
	{
		Error_Handler();
	}

	// Publicar en el tema público que la placa va a ser desconectada.
	Sw_MQTT_Write_PUBLISH(PUBLIC_TOPIC, sizeof(PUBLIC_TOPIC), MSG_DISCONNECTED, sizeof(MSG_DISCONNECTED));

	// Notificar al broker que se va cerrar la conexión.
	Sw_MQTT_Write_DISCONNECT();

	// Cerrar la conexión.
	Sw_MQTT_Close(mqtt_recv_buf, sizeof(mqtt_recv_buf));

	// Finalizar el programa.
	Error_Handler();
}


static void Error_Handler(void)
{
	// Bloquear el RTOS.
	osKernelLock();

	// Apagar los LEDs e indicar que el programa ha finalizado.
	Hw_LD1_Off();
	Hw_LD2_Off();
	Hw_LD3_Off();
	printf(" ------ Programa finalizado -------\n");

	// Bloquear aquí.
	for( ;; );
}
