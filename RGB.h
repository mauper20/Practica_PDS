#ifndef RGB_H_
#define RGB_H_

#include <GPIO.h>

//Valores para encendido/apagado
#define RED_LED 	0x00400000
#define BLUE_LED 	0x00200000
#define GREEN_LED 	0x04000000
//Puertos de LED
#define RED 	22
#define BLUE 	21
#define GREEN 	26

typedef enum {
	COLOR_WHITE, COLOR_GREEN, COLOR_BLUE, COLOR_PURPLE, COLOR_RED, COLOR_YELLOW
} led_color;

typedef enum {
	LED_ON, LED_OFF
} led_status;

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 Estas funciones ejecutan los pasos necesarios para configurar el puerto del led correspondiente
 */
void RGB_red_config(void);
void RGB_green_config(void);
void RGB_blue_config(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 Estas funciones encienden o apagan un color específico del led RGB
 */
void RGB_red_on(void);
void RGB_red_off(void);

void RGB_blue_on(void);
void RGB_blue_off(void);

void RGB_green_on(void);
void RGB_green_off(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 Estas funciones encienden o apagan una serie de colores específicos del led RGB
 */
void RGB_white_on(void);
void RGB_white_off(void);

void RGB_yellow_on(void);
void RGB_yellow_off(void);

void RGB_purple_on(void);
void RGB_purple_off(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 Genera un Delay de tiempo n
 */
void delay(uint32_t delay);

//Permite encender y apagar cada color sin importar el estado previo
void encender_led(led_color led);

//Configuracion del puerto de salida para led
void RGB_extern_output_config(gpio_port_name_t portName, uint8_t pin);

//Configuracion de interrupción
void RGB_PIT_config1s();

//Toogle para led externo, enciende 2 veces en 1 segundo
void RGB_2toogle_1s(gpio_port_name_t portName, uint8_t pin);

//Estados de leds
void RGB_estado1(gpio_port_name_t portName1, uint8_t pin1,gpio_port_name_t portName2, uint8_t pin2);
void RGB_estado2(gpio_port_name_t portName1, uint8_t pin1,gpio_port_name_t portName2, uint8_t pin2);
void RGB_estado3(gpio_port_name_t portName1, uint8_t pin1,gpio_port_name_t portName2, uint8_t pin2);


#endif /* RGB_H_ */
