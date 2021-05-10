/*
Este ejercicio medira el tiempo de pulsacion de un boton en donde un led comenzara a destellar con un periodo
de 1 segundo y cuyo tiempo de activacion sera el tiempo medido
*/

#include "main.h" //Incluimos el archivo "main.h"


//Definicion de pines, tiempos, nucleos a utilizar:
#define LED1        GPIO_NUM_26
#define PULSADOR1   GPIO_NUM_12
#define PERIODO_MS  1000 
#define PERIODO     pdMS_TO_TICKS(PERIODO_MS)
#define PROCESADORA 0
#define PROCESADORB 1
#define TICKSMAX    (PERIODO)
#define TW          5       //Cantidad de veces que se va a repetir el bucle antes de cambiar de estado, eliminando la posibilidad de rebote



//Tareas a utilizar:
void TareaDestello( void* taskParmPtr ); 
void TareaPulsador( void* taskParmPtr ); 


//Variables globales a utilizar:
int ASCENDENTE=0; 
int DESCENDENTE=0;
uint16_t duty = 0;
TickType_t conteoTicksInicio = 0;
TickType_t conteoTicksFinal = 0;


void app_main() {
    // Crear tarea en freeRTOS
    // Devuelve pdPASS si la tarea fue creada y agregada a la lista ready
    // En caso contrario devuelve pdFAIL.

    BaseType_t res1 = xTaskCreatePinnedToCore(
    	TareaDestello,                  // Funcion de la tarea a ejecutar
        "El led esta destellando",      // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE,       // Cantidad de stack de la tarea
        NULL,                           // Parametros de tarea
        tskIDLE_PRIORITY+1,             // Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                           // Puntero a la tarea creada en el sistema
        PROCESADORA                     //Procesador a utilizar
    );
    
    BaseType_t res2 = xTaskCreatePinnedToCore(
    	TareaPulsador,                  // Funcion de la tarea a ejecutar
        "Tarea contar tiempo pulsado",  // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE,       // Cantidad de stack de la tarea
        NULL,                           // Parametros de tarea
        tskIDLE_PRIORITY+1,             // Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                           // Puntero a la tarea creada en el sistema
        PROCESADORB                     //Procesador a utilizar
    );

//Gestion de los errores. (Si la tarea no se pudo crear se ingresa a un bucle infinito):
	if(res1 == pdFAIL)
	{
		printf( "Error al crear la tarea destellando.\r\n" );
		while(true);
	}
    if(res2 == pdFAIL)
	{
		printf( "Error al crear la tarea pulsador.\r\n" );
		while(true);
	}
}

// Implementacion de funcion de la tarea
void TareaDestello( void* taskParmPtr )
{
    // ---------- Configuraciones ------------------------------
    gpio_pad_select_gpio(LED1);
    gpio_set_direction(LED1, GPIO_MODE_OUTPUT);

    TickType_t xPeriodicity =  PERIODO;	// Tarea periodica cada 1000 ms

    TickType_t xLastWakeTime = xTaskGetTickCount();

    
    

    // ---------- Bucle infinito --------------------------
    while( true )
    {
        if ( duty > TICKSMAX )
        {
            duty = 0;
            printf("El tiempo pulsado supera al ciclo de trabajo \n");
        }

        else
        {
            gpio_set_level(LED1, 1);
            vTaskDelay (duty);
            gpio_set_level(LED1, 0);
        }
        
        vTaskDelayUntil( &xLastWakeTime , xPeriodicity ); //La tarea se bloque durante el xPeriodicity (1000 ms)
    }
}

void TareaPulsador( void* taskParmPtr )
{
    // ---------- Configuraciones ------------------------------
    gpio_pad_select_gpio(PULSADOR1);
    gpio_set_direction(PULSADOR1, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PULSADOR1, GPIO_PULLDOWN_ONLY);

    while( true )
    {
        switch (estadoActual)
         {
		    case ESTADO_ESPERA:
            {
			    if(gpio_get_level(PULSADOR1)==1)
                {
				 estadoActual = ESTADO_PULSADO;
			    }
		    }
		    break;
		    case ESTADO_PULSADO:{
                if(ASCENDENTE >= TW)
                {
		    	    conteoTicksInicio = xTaskGetTickCount();
                    estadoActual = ESTADO_ALTO;
                    ASCENDENTE = 0;
                }
                else
                {
                    estadoActual = ESTADO_ESPERA;
                    ASCENDENTE++;
                }
		    }
		    break;
		    case ESTADO_ALTO:{
		    	if(gpio_get_level(PULSADOR1)==0)
                {
                    if(DESCENDENTE >= TW)
                    {
                        conteoTicksFinal = xTaskGetTickCount();
                        duty = conteoTicksFinal-conteoTicksInicio;
                        estadoActual = ESTADO_ESPERA;
                        DESCENDENTE = 0;
                    }
                    DESCENDENTE++;
                }
		    }
		    break;

		    default:
            {
		    	estadoActual = ESTADO_ESPERA; //Reiniciamos si cae en un estado no valido
		    }
		    break;
         }
    }
}