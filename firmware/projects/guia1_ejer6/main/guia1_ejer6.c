/*! @mainpage Guia 1 ejercicio 6
 *
 * @section genDesc General Description
 *
 * El programa permite mostrar datos a traves de un display
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 *  * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	 	    	| 	GPIO_9		|
 * | 		    	| 	GPIO_18		|
 * | 	 	    	| 	GPIO_19		|
 * | 	  LCD 	    | 	GPIO_20		|
 * | 	 	    	| 	GPIO_21		|
 * | 		    	| 	GPIO_22		|
 * | 		    	| 	GPIO_23		|
 * | 	 Buzer	    | 	GPIO_3		|
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
#include "gpio_mcu.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*==================[macros and definitions]=================================*/

#define CONFIG_BLINK_PERIOD_MS 500u
#define MAX_DIGITS             3u
#define NUM_BCD_PINS           4u

typedef struct {
    gpio_t pin; /*!< Número de pin GPIO */
    io_t   dir; /*!< Dirección '0' IN; '1' OUT */
} gpioConf_t;

/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/

/**
 * @brief Convierte un entero decimal en un arreglo de dígitos (base 10).
 *
 * @param data       Valor decimal (hasta 32 bits).
 * @param digits     Cantidad de dígitos a generar (1..3).
 * @param bcd_array  Salida: dígitos con unidades en bcd_array[0].
 */
static void convert_to_bcd_array(uint32_t data, uint8_t digits, uint8_t *bcd_array)
{
    for (uint8_t i = 0; i < digits; i++) {
        bcd_array[i] = (uint8_t)(data % 10u); /* 0..9 */
        data /= 10u;
    }
}

/**
 * @brief Coloca un nibble BCD (0..9) en los 4 pines de datos.
 *
 * Mapeo esperado: b0→GPIO_20, b1→GPIO_21, b2→GPIO_22, b3→GPIO_23.
 *
 * @param bcd_nibble     Valor BCD (0..9).
 * @param data_pins      Arreglo de 4 pines de datos (salida).
 */
static void set_bcd_on_pins(uint8_t bcd_nibble, const gpioConf_t data_pins[NUM_BCD_PINS])
{
    /* Asegurar que los pines de datos estén inicializados como salida */
    for (uint8_t i = 0; i < NUM_BCD_PINS; i++) {
        GPIOInit(data_pins[i].pin, data_pins[i].dir);
    }

    for (uint8_t i = 0; i < NUM_BCD_PINS; i++) {
        if ((bcd_nibble >> i) & 0x1u) {
            GPIOOn(data_pins[i].pin);
        } else {
            GPIOOff(data_pins[i].pin);
        }
    }
}

/**
 * @brief Pulso corto de latch (>50 ns) en el pin de selección del dígito.
 *
 * @param sel_pin  Pin de selección del dígito (salida).
 */
static void lcd_latch_pulse(const gpioConf_t *sel_pin)
{
    /* Datos ya estables en D1..D4; generamos pulso ON->OFF */
    GPIOOn(sel_pin->pin);
    /* Pequeño retardo por software (~ decenas de ns en MCUs rápidas) */
    for (volatile int i = 0; i < 200; i++) { /* NOP dummy loop */ }
    GPIOOff(sel_pin->pin);
}

/**
 * @brief Muestra `value` usando `digits` dígitos del LCD con latch CD4543.
 *
 * @param value       Número decimal a mostrar.
 * @param digits      Cantidad de dígitos (1..3).
 * @param data_pins   Pines de datos BCD (4 elementos).
 * @param sel_pins    Pines de selección de dígito (3 elementos: dígito 1..3).
 */
static void lcd_show(uint32_t value,
                     uint8_t  digits,
                     const gpioConf_t data_pins[NUM_BCD_PINS],
                     const gpioConf_t sel_pins [MAX_DIGITS])
{
    uint8_t bcd[MAX_DIGITS] = {0};

    /* Inicializamos pines de selección como salida y en nivel bajo */
    for (uint8_t i = 0; i < MAX_DIGITS; i++) {
        GPIOInit(sel_pins[i].pin, sel_pins[i].dir);
        GPIOOff(sel_pins[i].pin);
    }

    convert_to_bcd_array(value, digits, bcd);

    /* Para cada dígito: colocar BCD en D1..D4 y dar pulso de latch en su SEL */
    for (uint8_t i = 0; i < digits; i++) {
    uint8_t idx = (uint8_t)(digits - 1u - i); // C, D, U
    set_bcd_on_pins(bcd[idx], data_pins);
    lcd_latch_pulse(&sel_pins[i]);
}

}

/**
 * @brief Alerta con buzzer si la cantidad de dígitos es inválida.
 */
static void beep_error(const gpioConf_t *buzzer_pin)
{
    GPIOInit(buzzer_pin->pin, buzzer_pin->dir);
    GPIOOn(buzzer_pin->pin);
    vTaskDelay(CONFIG_BLINK_PERIOD_MS / portTICK_PERIOD_MS);
    GPIOOff(buzzer_pin->pin);
}


/*==================[external functions definition]==========================*/

void app_main(void)
{
    /* Pines de datos BCD: b0..b3 -> GPIO_20..GPIO_23 */
    const gpioConf_t data_pins[NUM_BCD_PINS] = {
        { GPIO_20, GPIO_OUTPUT }, /* b0 */
        { GPIO_21, GPIO_OUTPUT }, /* b1 */
        { GPIO_22, GPIO_OUTPUT }, /* b2 */
        { GPIO_23, GPIO_OUTPUT }  /* b3 */
    };

    /* Pines de selección de dígito (según consigna) */
    const gpioConf_t sel_pins[MAX_DIGITS] = {
        { GPIO_19, GPIO_OUTPUT }, /* Dígito 1 */
        { GPIO_18, GPIO_OUTPUT }, /* Dígito 2 */
        { GPIO_9,  GPIO_OUTPUT }  /* Dígito 3 */
    };

    const gpioConf_t buzzer = { GPIO_3, GPIO_OUTPUT };

    /* Ejemplo de uso */
    uint32_t value  = 765;
    uint8_t  digits = 3;

    if (digits == 0u || digits > MAX_DIGITS) {
        beep_error(&buzzer);
        printf("La cantidad de dígitos ingresada no es correcta (1..3).\n");
        return;
    }

    lcd_show(value, digits, data_pins, sel_pins);
    printf("Mostrado %u en %u dígitos.\n", (unsigned)value, (unsigned)digits);
}

/*==================[end of file]============================================*/