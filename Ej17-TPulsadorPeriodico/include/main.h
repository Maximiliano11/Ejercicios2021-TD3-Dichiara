#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"

typedef enum
{
    ESTADO_ESPERA,
    ESTADO_PULSADO,
    ESTADO_ALTO,
} estadoMEF_t;

// Variable de estado (global):
estadoMEF_t estadoActual;