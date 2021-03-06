/*==================[ Inclusiones ]============================================*/
#include "pulsador.h"
#include "freertos/semphr.h" //librería para el uso de semaforos

/*==================[ Definiciones ]===================================*/

#define T_REBOTE_MS   40
#define T_REBOTE pdMS_TO_TICKS(T_REBOTE_MS)


/*==================[Prototipos de funciones]======================*/

static void errorPulsador( void );
static void botonLiberado( void );

void tareaPulsador( void* taskParmPtr );

/*==================[Variables]==============================*/
gpio_int_type_t pulsadorPines[1] = { GPIO_NUM_14 };

pulsadorInfo pulsadorA;

extern SemaphoreHandle_t semaforo;


void inicializarPulsador( void )
{
    pulsadorA.tecla             = pulsadorPines[0];
    pulsadorA.estado            = ALTO;     //Estado inicial

    gpio_pad_select_gpio(pulsadorA.tecla);
    gpio_set_direction(pulsadorA.tecla , GPIO_MODE_INPUT);
    gpio_set_pull_mode(pulsadorA.tecla, GPIO_PULLDOWN_ONLY); //Habilita resistencia de PULLDOWN interna

    // Crear tareas en freeRTOS
    BaseType_t res = xTaskCreatePinnedToCore(
    	tareaPulsador,                     	// Funcion de la tarea a ejecutar
        "tareaPulsador",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2,         // Cantidad de stack de la tarea
        NULL,                               // Parametros de tarea
        tskIDLE_PRIORITY+1,                 // Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          		// Puntero a la tarea creada en el sistema
        1                                   // Procesador a utilizar
    );

    // Gestion de errores:
	if(res == pdFAIL)
	{
		printf( "Error al crear la tarea.\r\n" );
		while(true);					    // Si no pudo crear la tarea queda en un bucle infinito
	}
}


static void errorPulsador( void )
{
    pulsadorA.estado = ALTO;
}


// pulsador_ Update State Function:
void actualizarPulsador()
{
    switch( pulsadorA.estado )
    {
        case BAJO:
            if( gpio_get_level( pulsadorA.tecla ) )
            {
                pulsadorA.estado = ASCENDENTE;
            }
            break;

        case ASCENDENTE:
            if( gpio_get_level( pulsadorA.tecla ) )
            {                                           
                pulsadorA.estado = ALTO;                
            }                                           
            
            else
            {
                pulsadorA.estado = BAJO;
            }
            break;

        case ALTO:
            if( !gpio_get_level( pulsadorA.tecla ) )
            {
                pulsadorA.estado = DESCENDENTE;
            }
            break;

        case DESCENDENTE:
            if( !gpio_get_level( pulsadorA.tecla ) )
            {
                pulsadorA.estado = BAJO;
                botonLiberado();
            }
            
            else
            {
                pulsadorA.estado = ALTO;
            }
            break;

        default:
            errorPulsador();
            break;
    }
}


static void botonLiberado()
{
    xSemaphoreGive( semaforo );       //Se libera la bandera del semáforo cuando se libera el pulsador
}


void tareaPulsador( void* taskParmPtr )
{
    while( true )
    {
        actualizarPulsador();
        vTaskDelay( T_REBOTE );
    }
}