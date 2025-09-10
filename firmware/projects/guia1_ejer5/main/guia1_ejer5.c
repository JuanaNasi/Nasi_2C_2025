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
 * | 03/09/2025 | Document creation		                         |
 *
 * @author Juana Nasi (jusnanasi3009@gmail.com)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "gpio_mcu.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/

typedef struct
{
	gpio_t pin;			/*!< GPIO pin number */
	io_t dir;			/*!< GPIO direction '0' IN;  '1' OUT*/
} gpioConf_t;

void ModificarEstadoPinCodificacion(uint8_t BCD, gpioConf_t *Pines)
{
	if (Pines == NULL) {
        printf("Error: vector de pines nulo.\n");
        return;
    }

    /* Asegurar uso de 4 bits (BCD) */
    BCD &= 0x0F;

    /* Aviso opcional si no es BCD decimal estricto (0..9) */
    if (BCD > 9) {
        printf("Aviso: valor %u no es BCD decimal estricto (0..9). Se escriben igual los 4 bits.\n",
               (unsigned)BCD);
    }

    /* Inicializo los pines según su configuración */
    for (uint8_t i = 0; i < 4; i++)
    {
        GPIOInit(Pines[i].pin, Pines[i].dir);
    }

    /* Escribo cada bit b0..b3 en el pin correspondiente */
    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t bit = (BCD >> i) & 0x01;
        if (bit) {
            GPIOOn(Pines[i].pin);
        } else {
            GPIOOff(Pines[i].pin);
        }
    }
}

/*==================[external functions definition]==========================*/


void app_main(void)
{
    uint8_t NumeroBCD = 7;   /* 0b0111 -> b0=1 b1=1 b2=1 b3=0 */
    gpioConf_t Pin[4];

    /* Configuro los pines: b0->GPIO_20, b1->GPIO_21, b2->GPIO_22, b3->GPIO_23 */
    Pin[0].pin = GPIO_20;  Pin[0].dir = GPIO_OUTPUT;  /* b0 */
    Pin[1].pin = GPIO_21;  Pin[1].dir = GPIO_OUTPUT;  /* b1 */
    Pin[2].pin = GPIO_22;  Pin[2].dir = GPIO_OUTPUT;  /* b2 */
    Pin[3].pin = GPIO_23;  Pin[3].dir = GPIO_OUTPUT;  /* b3 */

    ModificarEstadoPinCodificacion(NumeroBCD, Pin);

    /* (Opcional) Depuración */
    printf("Se escribio BCD=%u (0x%X) en GPIO_20..GPIO_23.\n",
           (unsigned)NumeroBCD, (unsigned)NumeroBCD);
}
/*==================[end of file]============================================*/