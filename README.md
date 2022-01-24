# NUCLEO_H723ZG_MQTT_TLS

Aplicación para la placa de desarrollo ``NUCLEO_H723ZG`` de STMicroelectronics que hace uso de unos clientes desarrollados por el autor de este repositorio para los siguientes protocolos de capa de aplicación:

| Protocolo | Función                                                          |
| --------- | ---------------------------------------------------------------- |
| DHCP      | Establecer la dirección IP, máscara de subred y puerta de enlace |
| DNS       | Resolver los hostnames del servidor NTP y del broker MQTT        |
| NTP       | Obtener la fecha y hora                                          |
| TLS       | Asegurar la conexión                                             |
| MQTT      | Enviar y recibir mensajes                                        |

## Funcionamiento

La aplicación inicialmente establece la dirección IP, máscara de subred y puerta de enlace. Luego obtiene la fecha y hora. A continuación conecta con el broker MQTT y se suscribe a un tema secreto.

Tras esta inicialización, la aplicación publica periódicamente la fecha y hora en un tema público, mientras escucha los mensajes que puedan publicar otros clientes en el tema secreto. Al recibir un mensaje, la aplicación comprueba si es un comando para encender o apagar los LEDs de la placa de desarrollo y, de ser así, actúa en consecuencia. Los comandos aceptados son los siguientes:

|          | LED 1      | LED 2      | LED 3      |
| -------- | ---------- | ---------- | ---------- |
| Encender | ``LD1ON``  | ``LD2ON``  | ``LD3ON``  |
| Apagar   | ``LD1OFF`` | ``LD2OFF`` | ``LD3OFF`` |

Se puede finalizar el programa accionando el pulsador azul tras completar la inicialización de la aplicación, es decir, a partir del momento en el que la aplicación puede enviar y recibir mensajes MQTT.

La aplicación permite asegurar la conexión mediante TLS. Para ello ha de contarse con el certificado del CA raíz del broker y la fecha y hora correcta para poder verificar la cadena de confianza. Además, la aplicación también permite añadir un par de claves y certificado propios, para conexiones que requieran de autenticación del cliente por parte del broker. Esto último se encuentra deshabilitado por defecto.

Los detalles del funcionamiento de la aplicación se encuentran en los comentarios del archivo ``main.c``.

## Configuración

Para configurar la aplicación pueden modificarse los siguientes archivos:

| Archivo            | Contenido                                                                                                                                    |
| ------------------ | -------------------------------------------------------------------------------------------------------------------------------------------- |
| ``settings.h``     | Configuración general de la aplicación (por ejemplo, servidores a los que conecta, si se usa una conexión TLS o el nombre de los temas MQTT) |
| ``certificates.h`` | Certificado del CA raíz del broker                                                                                                           |
|                    | Par de claves y certificado del cliente                                                                                                      |

## Compilación

Este proyecto de programación está listo para ser compilado con ``STM32CubeIDE``.

## Depuración

Todos los sucesos se pueden logear a través de la UART (habilitada por defecto) con la siguiente configuración en ``PuTTY`` (versión usada: 0.73):

``- Session    -> Connection type: Serial                         ``

``- Session    -> Logging -> All session output (e indicar ruta)  ``

``- Terminal   -> Implicit CR in every LF                         ``

``- Connection -> Serial  -> Serial line to connect to (la del PC)``

``- Connection -> Serial  -> Speed (baud) : 115200                ``

``- Connection -> Serial  -> Data bits    :      8                ``

``- Connection -> Serial  -> Stop bits    :      1                ``

``- Connection -> Serial  -> Parity       :   None                ``

``- Connection -> Serial  -> Flow control :   None                ``

## Librerías

En el proyecto de programación son utilizadas librerías de los siguientes repositorios:

| Repositorio                                                                | Versión | Uso en el proyecto de programación             |
| -------------------------------------------------------------------------- | ------- | ---------------------------------------------- |
| [lwIP](http://download.savannah.nongnu.org/releases/lwip/)                 | 2.1.2   | Stack de TCP/IP                                |
| [mbedTLS](https://github.com/ARMmbed/mbedtls/releases/)                    | 2.16.11 | Stack de TLS                                   |
|                                                                            |         | Infraestructura de certificados                |
| [FreeRTOS](https://github.com/FreeRTOS/FreeRTOS/releases/)                 | 10.4.4  | Sistema operativo de tiempo real               |
| [CMSIS](https://github.com/ARM-software/CMSIS_5/releases/)                 | 5.8.0   | Parte 1 de la capa de abstracción de hardware  |
| [STM32CubeH7](https://github.com/STMicroelectronics/STM32CubeH7/releases/) | 1.9.0   | Parte 2 de la capa de abstracción de hardware  |
|                                                                            |         | Parte 1 del wrapper sobre FreeRTOS             |
| [CMSIS-FreeRTOS](https://github.com/ARM-software/CMSIS-FreeRTOS/releases/) | 10.3.1  | Parte 2 del wrapper sobre FreeRTOS             |

Los archivos necesarios de las librerías se encuentran dentro del proyecto de programación.

## Licencias

Distintos componentes de este proyecto de programación se distribuyen bajo diferentes licencias, que se encuentran el archivo ``LICENSE.txt``.
