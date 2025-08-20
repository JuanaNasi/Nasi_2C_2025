/*! @mainpage Template
 *
 * @section genDesc General Description
 *
 * This section describes how the program works.
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	PIN_X	 	| 	GPIO_X		|
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 20/08/2025 | Document creation		                         |
 *
 * @author Juana Nasi (juananasi3009@gmail.com)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h> /*si quiero usar True o False*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "switch.h"
/*==================[macros and definitions]=================================*/

#define ON 0
#define OFF 1
#define TOOGLE 2
#define CONFIG_BLINK_PERIOD 100

/*==================[internal data definition]===============================*/
struct leds
{
    uint8_t mode;       //ON, OFF, TOGGLE
	uint8_t n_led;        //indica el número de led a controlar
	uint8_t n_ciclos;   //indica la cantidad de ciclos de encendido/apagado
	uint16_t periodo;    //indica el tiempo de cada ciclo
} my_leds;

/*==================[internal functions declaration]=========================*/
void ManejarLeds(struct leds *my_leds){
	switch (my_leds->mode)
	{
	case ON:
		switch(my_leds->n_led)
		{
			case LED_1:
	    		LedOn(LED_1);
				break;
	   		case LED_2:
				LedOn(LED_2);
				break;
	    	case LED_3:
	    		LedOn(LED_3);
				break;
			default:
					break;	
		}
		break;
	case OFF:
		switch(my_leds->n_led)
		{
			case LED_1:
	    		LedOff(LED_1);
				break;
	   		case LED_2:
				LedOff(LED_2);
				break;
	    	case LED_3:
	    		LedOff(LED_3);
				break;
			default:
					break;
	    
		}	
		break;
	case TOOGLE:
		for(int j=0; j<my_leds->n_ciclos;j++)
		{
			switch(my_leds->n_led)
			{
				case LED_1:
	    			LedToggle(LED_1);
					break;
	   		 	case LED_2:
					LedToggle(LED_2);
					break;
	    		case LED_3:
	    			LedToggle(LED_3);
					break;
	    		default:
					break;
			}	
			
			for(int j=0; j<my_leds->periodo/100;j++)
			{
				vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
			}
		}
		break;

	}
}

/*==================[external functions definition]==========================*/
void app_main(void)
{
	
	//SystemClockInit();
	LedsInit();
	//SwitchesInit();

	struct leds led;
	led.n_led = LED_3;
	led.periodo = 500;
	led.n_ciclos = 10;
	led.mode = TOOGLE;

	ManejarLeds(&led);

}

/*==================[end of file]============================================*/