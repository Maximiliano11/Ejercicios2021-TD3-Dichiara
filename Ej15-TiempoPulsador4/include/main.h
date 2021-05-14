#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"


//Definicion de pines, tiempos, nucleos a utilizar:
#define LED1        GPIO_NUM_26
#define PULSADOR1   GPIO_NUM_12
#define PROCESADORA 0
#define PROCESADORB 1
#define TW          5 //Cantidad de veces que se va a repetir el bucle antes de cambiar de estado, eliminando la posibilidad de rebote


//Funciones y tareas a utilizar:
void TareaPulsador( void* taskParmPtr );
void TareaSalida( void* taskParmPtr ); 
void InicializarIOs();
void ActualizarIO();
void PrenderSalida();
void BorrarTarea();
void CrearTareaSalida();


typedef enum
{
    ESTADO_ESPERA,
    ESTADO_PULSADO,
    ESTADO_ALTO,
} estadoMEF_t;


// Variable de estado (global):
estadoMEF_t estadoActual;
TaskHandle_t xHandle1 = NULL; 