#define LED1 GPIO_NUM_32 //ver de crear un tipo de datos
#define LED2 GPIO_NUM_33
#define LED3 GPIO_NUM_25
#define PULSADOR1 GPIO_NUM_26
#define PULSADOR2 GPIO_NUM_27
#define T_ON		500/portTICK_PERIOD_MS
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
   int i = 0;
   unsigned int sentido = 0;
   while( true )
    {
	   	if (gpio_get_level(PULSADOR1) == 0) //Con el gpio_get_level vamos a poder leer un pin 
		{
			sentido = 1;
		}
	   	else if (gpio_get_level(PULSADOR2) == 0)
		{
			sentido = 0;
		}
		if ( i == 0 ){
			gpio_set_level(LED1, 1); //Si i=0 se prendera el led 1 y los otros no
			gpio_set_level(LED2, 0);
			gpio_set_level(LED3, 0);
		}
		if ( i == 1 ){
			gpio_set_level(LED1, 0);
			gpio_set_level(LED2, 1); //Si i=1 se prendera el led del medio y los otros no
			gpio_set_level(LED3, 0);
		}
		if ( i == 2 ){
			gpio_set_level(LED1, 0);
			gpio_set_level(LED2, 0);
			gpio_set_level(LED3, 1); //Si i=2 se prendera el ultimo led y los otros no
		}
		if(sentido == 0){
			i++; //Incrementamos si el sentido es cero para ir encenciendo cada led
		}
		else{
			i--; //Decrementamos si el sentido es uno y asi la secuencia sera alreves
		}

		if ( i < 0 ){
		i = 2; //Si i es menos a cero sera igual a 2
		}
		if ( i > 2 ){
		i = 0; //Si i es mayor a 2 sera igual a cero
		}
		vTaskDelay(T_ON);
	} //De esta forma se mantendra la secuencia de encendido entre los 3 leds
   return 0;
}