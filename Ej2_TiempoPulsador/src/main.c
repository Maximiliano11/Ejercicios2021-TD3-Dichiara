/* Este programa mide el tiempo que se mantiene presionado un pulsador, cuando el mismo se suelta se encendeera
un led y se mantendrea encendido durante el mismo tiempo que el pulsador estuvo presionado */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h" //Poner siempre estas 3 librerias
#include "driver/gpio.h"
#include "sdkconfig.h"

#define N_LED 		2 //Numero de leds que se utilizaran
#define T_MS		10/portTICK_PERIOD_MS //Esto me permitira poner mi delay (ver algunas lineas mas abajo) en milisegundos, el tiempo es el recomendado en el enunciado del ejercicio


//Arreglos para facilitar el agregado de leds y pulsadores, podemos agregar todos los que nosotros queramos
int led [N_LED] = {GPIO_NUM_32, GPIO_NUM_33}; //Pines 32 y 33 usados como salidas para encender los leds
int pulsador [N_LED] = {GPIO_NUM_26, GPIO_NUM_27}; //Pines 26 y 27 usados como entradas para los pulsadores
unsigned long int contador[N_LED] = { 0, 0}; //Almacenan el tiempo

void app_main()
{
	
	//Configuración, esto es como en el ejercicio 1
	for(int i = 0; i < N_LED; i++){
		gpio_pad_select_gpio(led[i]); //Elige un pin desde el IOMUX
		gpio_set_direction(led[i], GPIO_MODE_OUTPUT); //Configura el pin como salida digital
	}
	
	for(int i = 0; i < N_LED; i++){
		gpio_pad_select_gpio(pulsador[i]);
		gpio_set_direction(pulsador[i], GPIO_MODE_INPUT); //Configura el pin como entrada digital
		gpio_set_pull_mode(pulsador[i], GPIO_PULLDOWN_ONLY); //Habilita resistencia de PULLDOWN interna
	}

   //Dentro de este while, tendremos otros 2 mas, uno lee el pulsador cero y otro lee el pulsador 1
   while( true )
    {
		while (gpio_get_level(pulsador[0]) == 1) //Mientras el pulsador esté habilitado, incrementa el contador
		{
			vTaskDelay(T_MS); //Delay con un tiempo T en milisegundos (Lineas mas arriba se definio que es de 10 milisegundos)
			contador[0] += T_MS; //El contador recibe el tiempo
		} //El while se va a ejecutar mientras yo mantenga presionado el pulsador

	   	while (gpio_get_level(pulsador[1]) == 1) //Aca ocurre lo mismo para el otro pulsador
		{
			vTaskDelay(T_MS);
			contador[1] += T_MS;
		}	

		//Si algun contador es mayor a 0, habilita el led correspondiente
	   	if (contador[0] >0)
		{
			gpio_set_level(led[0], 1); //Se enciende el led
			vTaskDelay(contador[0]); 
			gpio_set_level(led[0], 0);
            contador[0] = 0;
		}
	   	if (contador[1] >0)
		{
			gpio_set_level(led[1], 1); //Se enciende el otro led
			vTaskDelay(contador[1]);
			gpio_set_level(led[1], 0);
            contador[1] = 0;
		}
	}
}
/* Nota: me hubiese gustado reemplazar estos ultimos if por un switch case, pero no encontre si se puede
poner la estructura switch dentro del bucle while, o si ese bucle se tendria que eliminar,
en resumen... no supe como hacer dicha modificacion*/