#include <stdio.h>
#include <string.h> //Se usa para cadenas de texto
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h" //Esta linea y la anterior ponerla siempre
#include "driver/gpio.h" //Controla los pines como entrada o salida

//No olvidarse las librerias!!

#define LED1 GPIO_NUM_32 //ver de crear un tipo de datos
#define LED2 GPIO_NUM_33
#define LED3 GPIO_NUM_25
#define PULSADOR1 GPIO_NUM_26
#define PULSADOR2 GPIO_NUM_27
#define T_ON		500/portTICK_PERIOD_MS
#define derecha 0
#define izquierda 1

int estado=1; //Al arrancar en 1 empieza con 1 led encendido (si arranco en cero empieza con todos los leds apagados)
unsigned int sentido = 0;
void secuencia();

void app_main()
{
	//Las funciones que estan a continuacion se sacaron del ejemplo del Blink (programa que enciende y apaga un Led) en donde se permite calcular las entradas y salidas de manera directa
	gpio_pad_select_gpio(LED1); //Seleccionamos con que puerto queremos trabajar
	gpio_set_direction(LED1, GPIO_MODE_OUTPUT); //Con el set_direction elegimos si el puerto va a funcionar como entrada o salida (los leds seran salidas y los pulsadores entradas)
	gpio_pad_select_gpio(LED2);
	gpio_set_direction(LED2, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(LED3);
	gpio_set_direction(LED3, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(PULSADOR1);
	gpio_set_direction(PULSADOR1, GPIO_MODE_INPUT);
	gpio_pad_select_gpio(PULSADOR2);
	gpio_set_direction(PULSADOR2, GPIO_MODE_INPUT);
   //Hasta aca ya hemos configurado todos los pines que vamos a utilizar

   //Ahora creamos un bucle while que se ejecutara por tiempo indefinido
   while(1)
    {
		if (gpio_get_level(PULSADOR1)) 
		{
			sentido=izquierda;
		}
		if (gpio_get_level(PULSADOR2))
		{
			sentido=derecha;
		}
		
		secuencia();
		vTaskDelay(T_ON);
		
		if (sentido==derecha) //Gira a la derecha
		{
			if (estado==3)
			{
				estado=0;  
			}
			estado++; 
		}
		if (sentido==izquierda)
		{
			if (estado==1)
			{
				estado=4; 
			}
			
			estado--; //Si no se ejecuta el if, decrementa e invierte el sentido
		}
	}
}

void secuencia() 
{
	switch (estado)
	{
		case '1':
		gpio_set_level(LED1, 1); //Si i=0 se prendera el led 1 y los otros no
		gpio_set_level(LED2, 0);
		gpio_set_level(LED3, 0);
		break;
		case '2':
		gpio_set_level(LED1, 0); 
		gpio_set_level(LED2, 1); //Si i=0 se prendera el led 2 y los otros no
		gpio_set_level(LED3, 0);
		break;
		case '3':
		gpio_set_level(LED1, 0); 
		gpio_set_level(LED2, 0);
		gpio_set_level(LED3, 1); //Si i=0 se prendera el led 3 y los otros no	
		break;
		default: //Con este default, lo que se logra es que en caso de que haya un error se apagaran todos los leds
		gpio_set_level(LED1, 0); 
		gpio_set_level(LED2, 0);
		gpio_set_level(LED3, 0);
		break;
	}
}
