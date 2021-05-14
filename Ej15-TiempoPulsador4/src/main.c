/*
En este ejercicio, al presionar la tecla se deberá crear una tareaLed que encienda el led correspondiente,
luego se apague y la tarea se autodestruya, liberando la memoria asociada a la misma.
*/


#include "main.h" //Incluimos el archivo "main.h"


//Variables globales:
int ASCENDENTE=0;
int DESCENDENTE=0;
uint32_t duty = 0;
TickType_t conteoTicksInicio = 0;
TickType_t conteoTicksFinal = 0;


void app_main() 
{
    InicializarIOs();   
}


void InicializarIOs()
{
    gpio_pad_select_gpio(PULSADOR1);
    gpio_set_direction(PULSADOR1, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PULSADOR1, GPIO_PULLDOWN_ONLY);

    gpio_pad_select_gpio(LED1);
    gpio_set_direction(LED1, GPIO_MODE_OUTPUT);

    BaseType_t res = xTaskCreatePinnedToCore(
    	    TareaPulsador,                  // Funcion de la tarea a ejecutar
            "Tarea contar tiempo pulsado",  // Nombre de la tarea como String amigable para el usuario
            configMINIMAL_STACK_SIZE, 		// Cantidad de stack de la tarea
            NULL,                          	// Parametros de tarea
            tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
            NULL,                           // Puntero a la tarea creada en el sistema
            PROCESADORA                     // Procesador a utilizar
      );

      if(res == pdFAIL)
    	{
    		printf( "Error al crear la tarea pulsador.\r\n" );
    		while(true);					// Si no pudo crear la tarea queda en un bucle infinito
    	}
}



void TareaPulsador( void* taskParmPtr )
{
    ActualizarIO();
    vTaskDelay(10 /portTICK_PERIOD_MS);
}

void TareaSalida( void* taskParmPtr )
{
    PrenderSalida();
    printf("La tarea se borrara automaticamente \n"); //La tarea se autodestruye
    BorrarTarea(xHandle1);
}


void PrenderSalida()
{
    gpio_set_level(LED1, 1);
    vTaskDelay (duty);
    gpio_set_level(LED1, 0);
}

void CrearTareaSalida()
{
    BaseType_t res2 = xTaskCreatePinnedToCore(
    	    TareaSalida,                    // Funcion de la tarea a ejecutar
            "Tarea prender salida",         // Nombre de la tarea como String amigable para el usuario
            configMINIMAL_STACK_SIZE, 		// Cantidad de stack de la tarea
            NULL,                          	// Parametros de tarea
            tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
            xHandle1,                       // Puntero a la tarea creada en el sistema
            PROCESADORB                     // Procesador a utilizar
      );
      
      if(res2 == pdFAIL)
    	{
    		printf( "Error al crear la tarea salida.\r\n" );
    		while(true);					// Si no pudo crear la tarea queda en un bucle infinito
    	}
}

void BorrarTarea()
{
    vTaskDelete(xHandle1);
}

void ActualizarIO()
{
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

		    case ESTADO_PULSADO:
            {
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

		    case ESTADO_ALTO:
            {
		    	if(gpio_get_level(PULSADOR1)==0)
                {
                    if(DESCENDENTE >= TW)
                    {
                        conteoTicksFinal = xTaskGetTickCount();
                        duty = conteoTicksFinal-conteoTicksInicio;
                        CrearTareaSalida();
                        estadoActual = ESTADO_ESPERA;
                        DESCENDENTE = 0;
                    }
                    else 
                    {
                    DESCENDENTE++;
                    }
                }
		    }
		    break;

		    default:
            {
		    	//Si cae en un estado no valido, reinicio
		    	estadoActual = ESTADO_ESPERA;
		    }
		    break;
	    }   
    }
}