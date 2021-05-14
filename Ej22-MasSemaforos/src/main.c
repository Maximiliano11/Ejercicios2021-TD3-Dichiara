/*
Este programa debera tener 2 tareas, en donde una implemente un
pulsador con antirebote y libere un semaforo cuando el pulsador se suelte.

Otra tarea espera el semaforo cada segundo, si recibe el semaforo
destella un led verde, sino uno rojo
*/


#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "pulsador.h"
#include "freertos/semphr.h" //librería para el uso de semaforos

#define SALIDA1     GPIO_NUM_25 //Led 1
#define SALIDA2     GPIO_NUM_26 //Led 2
#define T_ESPERA_MS  40
#define T_ESPERA     pdMS_TO_TICKS(T_ESPERA_MS)
#define PROCESADORA 0
#define PROCESADORB 1
#define LED_ON_MS    500
#define LED_ON       pdMS_TO_TICKS(LED_ON_MS)

void tareaDestello( void* taskParmPtr ); //Prototipo de la función de la tarea

SemaphoreHandle_t semaforo = NULL; //Puntero al semaforo

void app_main()
{
    //Crear el semáforo (arranca “tomado”)
    semaforo = xSemaphoreCreateBinary();
    if(semaforo == NULL)
    {
        printf("No se pudo crear el semaforo\n");
        while(true);
    }
    // Crear tarea en freeRTOS
    // Devuelve pdPASS si la tarea fue creada y agregada a la lista ready
    // En caso contrario devuelve pdFAIL.
    inicializarPulsador();

    BaseType_t res = xTaskCreatePinnedToCore(
    	tareaDestello,                     	// Funcion de la tarea a ejecutar
        "tareaDestello",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE, 		// Cantidad de stack de la tarea
        NULL,                          	// Parametros de tarea
        tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          		// Puntero a la tarea creada en el sistema
        PROCESADORA
    );

    // Gestion de errores
	if(res == pdFAIL)
	{
		printf( "Error al crear la tarea.\r\n" );
		while(true);					// si no pudo crear la tarea queda en un bucle infinito
	}
}

// Implementacion de funcion de la tarea
void tareaDestello( void* taskParmPtr )
{
    // ---------- Configuraciones ------------------------------
    gpio_pad_select_gpio(SALIDA1);
    gpio_set_direction(SALIDA1, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(SALIDA2);
    gpio_set_direction(SALIDA2, GPIO_MODE_OUTPUT);

    // ---------- Bucle infinito --------------------------
    while( true )
    {
        xSemaphoreTake( semaforo , portMAX_DELAY);

        gpio_set_level( SALIDA1, 1 );
        vTaskDelay( LED_ON );
        gpio_set_level( SALIDA1, 0 );
    }
}