/*! @mainpage Blinking
 *
 * \section genDesc General Description
 *
 * This example makes LED_1 blink.
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 12/09/2023 | Document creation		                         |
 *
 * @author Juana Nasi (juananasi3009@gmail.com)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
/*==================[macros and definitions]=================================*/
#define CONFIG_BLINK_PERIOD 1000
/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/

/*==================[external functions definition]==========================*/
void app_main(void){
    LedsInit();
    while(true){
        printf("LED ON 1\n");
        LedOn(LED_1);
        printf("LED ON 2\n");
        LedOn(LED_2);
        printf("LED ON 3\n");
        LedOn(LED_3);
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        printf("LED OFF 2\n");
        LedOff(LED_1);
        printf("LED OFF 2\n");
        LedOff(LED_2);
         printf("LED OFF 3\n");
        LedOff(LED_3);
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}
/*==================[end of file]============================================*/
