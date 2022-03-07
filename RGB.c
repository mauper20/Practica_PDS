#include "RGB.h"
#include "MK64F12.h"
#include "GPIO.h"
#include "Bits.h"
#include "NVIC.h"
#include "PIT.h"

#define SYSTEM_CLOCK (21000000U)
#define DELAYLED (0.50F)

gpio_pin_control_register_t RGB_input_intr_config = GPIO_MUX1;	//Configuración del GPIO

void RGB_red_config(void) {
	GPIO_clock_gating(GPIO_B);										//Activa el puerto B
	GPIO_pin_control_register(GPIO_B, RED, &RGB_input_intr_config); //Configuracion del GPIO B para el puerto del led rojo
	GPIO_write_port(GPIO_B, RED_LED);								//Escribe un valor seguro
	GPIO_data_direction_port(GPIO_B, RED_LED);						//Configura el puerto del led azul del GPIO B como Output
}
void RGB_green_config(void) {
	GPIO_clock_gating(GPIO_E);										  //Activa el puerto E
	GPIO_pin_control_register(GPIO_E, GREEN, &RGB_input_intr_config); //Configuracion del GPIO E para el puerto del led verde
	GPIO_write_port(GPIO_E, GREEN_LED);								  //Escribe un valor seguro
	GPIO_data_direction_port(GPIO_E, GREEN_LED);					  //Configura el puerto del led azul del GPIO E como Output
}
void RGB_blue_config(void) {
	GPIO_clock_gating(GPIO_B);										 //Activa el puerto B
	GPIO_pin_control_register(GPIO_B, BLUE, &RGB_input_intr_config); //Configuracion del GPIO B para el puerto del led azul
	GPIO_write_port(GPIO_B, BLUE_LED);								 //Escribe un valor seguro
	GPIO_data_direction_port(GPIO_B, BLUE_LED);						 //Configura el puerto del led azul del GPIO B como Output
}

void RGB_red_on(void) {
	GPIO_clear_pin(GPIO_B, RED);	  //enciende el led rojo
}

void RGB_red_off(void) {
	GPIO_write_port(GPIO_B, RED_LED); //apaga el led verde
}

void RGB_blue_on(void) {
	GPIO_clear_pin(GPIO_B, BLUE);	  //enciende el led azul
}

void RGB_blue_off(void) {
	GPIO_write_port(GPIO_B, BLUE_LED);//apaga el led verde
}

void RGB_green_on(void) {
	GPIO_clear_pin(GPIO_E, GREEN);	  //enciende el led verde
}

void RGB_green_off(void) {
	GPIO_write_port(GPIO_E, GREEN_LED);//apaga el led verde
}

void RGB_white_on(void) {
	//Para generar blanco se necesita un valor RGB=(255,255,255)
	RGB_green_on();
	RGB_red_on();
	RGB_blue_on();
}

void RGB_white_off(void) {
	//apaga todos los colores
	RGB_green_off();
	RGB_red_off();
	RGB_blue_off();
}

void RGB_yellow_on(void) {
	//Para generar amarillo se necesita un valor RGB=(255,255,0)
	RGB_green_on();
	RGB_red_on();
}

void RGB_yellow_off(void) {
	//Apaga color amarillo
	RGB_green_off();
	RGB_red_off();
}

void RGB_purple_on(void) {
	//Para generar morado se necesita un valor RGB=(255,0,255)
	RGB_red_on();
	RGB_blue_on();
}

void RGB_purple_off(void) {
	//Apaga color morado
	RGB_red_off();
	RGB_blue_off();
}

//Permite encender y apagar cada color sin importar el estado previo
void encender_led(led_color led) {
	switch (led) {
	case COLOR_GREEN:
		RGB_white_off();
		RGB_green_on();
		break;
	case COLOR_BLUE:
		RGB_white_off();
		RGB_blue_on();
		break;
	case COLOR_PURPLE:
		RGB_white_off();
		RGB_purple_on();
		break;
	case COLOR_RED:
		RGB_white_off();
		RGB_red_on();
		break;
	case COLOR_YELLOW:
		RGB_white_off();
		RGB_yellow_on();
		break;
	case COLOR_WHITE:
		RGB_white_off();
		RGB_white_on();
		break;
	default:
		break;
	}
}

//Permite configurar un puerto como output para un led externo

void RGB_extern_output_config(gpio_port_name_t portName, uint8_t pin) {
	gpio_pin_control_register_t pin_control_register_bit_d_0 = GPIO_MUX1;
	GPIO_clock_gating(portName);
	GPIO_pin_control_register(portName, pin, &pin_control_register_bit_d_0);
	GPIO_data_direction_pin(portName, GPIO_OUTPUT, pin);
	GPIO_set_pin(portName, pin);
}

//Genera una interrupción para el toogle
void RGB_PIT_config1s() {
	PIT_clock_gating();
	PIT_enable();
	NVIC_set_basepri_threshold(PRIORITY_12);
	NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_3);
	NVIC_global_enable_interrupts;
	PIT_delay(PIT_2, SYSTEM_CLOCK, DELAYLED);
	PIT_enable_interrupt_2(PIT_2);
}

void RGB_2toogle_1s(gpio_port_name_t portName, uint8_t pin) {
	uint8_t i = 3;
	uint8_t pit_inter_status = FALSE;
	led_status flag_led = LED_ON;

	RGB_extern_output_config(portName, pin);
	RGB_PIT_config1s();
	GPIO_clear_pin(portName, pin);
	for (i = 3; i != 0; i--) {
		do {
			pit_inter_status = PIT_get2_interrupt_flag_status();
		} while (FALSE == pit_inter_status);

		if (LED_ON == flag_led) {
			GPIO_set_pin(portName, pin);
			flag_led = LED_OFF;
		} else {
			GPIO_clear_pin(portName, pin);
			flag_led = LED_ON;
		}

		PIT_clear2_interrupt_flag();

	}
}

//estados
void RGB_estado1(gpio_port_name_t portName1, uint8_t pin1,
		gpio_port_name_t portName2, uint8_t pin2) {
	RGB_extern_output_config(portName1, pin1);
	RGB_extern_output_config(portName2, pin2);
	GPIO_set_pin(portName1, pin1);
	GPIO_set_pin(portName2, pin2);
	GPIO_clear_pin(portName2, pin2);
}

void RGB_estado2(gpio_port_name_t portName1, uint8_t pin1,
		gpio_port_name_t portName2, uint8_t pin2) {
	RGB_extern_output_config(portName1, pin1);
	RGB_extern_output_config(portName2, pin2);
	GPIO_set_pin(portName1, pin1);
	GPIO_set_pin(portName2, pin2);
	GPIO_clear_pin(portName1, pin1);
}
void RGB_estado3(gpio_port_name_t portName1, uint8_t pin1,
		gpio_port_name_t portName2, uint8_t pin2) {
	RGB_extern_output_config(portName1, pin1);
	RGB_extern_output_config(portName2, pin2);
	GPIO_set_pin(portName1, pin1);
	GPIO_set_pin(portName2, pin2);
	GPIO_clear_pin(portName1, pin1);
	GPIO_clear_pin(portName2, pin2);
}
void delay(uint32_t delay)//k64 va a 21MHz
{
	volatile uint32_t j;
	for(j = delay; j > 0; j--)
	{
		__asm("nop");
	}
}

