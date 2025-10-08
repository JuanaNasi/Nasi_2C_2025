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

/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/

void  ConvertToBcdArray (uint32_t data, uint8_t digits, uint8_t * bcd_number) //los uint indican cuantos bits ocupa la variable y si admite numeros negativos o no
{
		uint8_t resto; // u=unsigned y int=entero, _t=tipo
		for(uint8_t i = 0; i<digits; i++)
		{
			resto = data%10; //Mediante el operador % obtengo el dato del resto del cociente. Te da el último dígito del número.
            bcd_number[i] = resto; //Guardo dicho resto para obtener la codificacion de dich digito decimal en BCD
			data /= 10; //Elimina ese último dígito para poder seguir con el próximo.
		}
}

/*==================[external functions definition]==========================*/
void app_main(void)
{
	
	uint32_t dato = 123;
	uint8_t cant_digitos = 3;
	uint8_t NumeroBCD[cant_digitos];

	if (cant_digitos >= 10) /*Un número de 32 bits sin signo puede representar valores de máximo 10 dígitos decimales. 
	Con 10 dígitos alcanza para cubrir todo el rango de uint32_t */ 
	{
		printf("Ingresaste un numero con una cantidad de digitos incorrecta. Vuele a intentarlo!\n");
		
	}

	else 
	{
		ConvertToBcdArray (dato, cant_digitos, NumeroBCD);

		for(uint8_t i = 0; i<cant_digitos; i++)
		{
			printf("BCD[%d]: %d\n", i, NumeroBCD[i]);
			
		}
	}

}
/*==================[end of file]============================================*/