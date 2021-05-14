#ifndef LED_H_
#define LED_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void Iniciar_Led( void );

void Encender_Led( void );

#endif
