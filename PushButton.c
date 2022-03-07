#include "PushButton.h"
#include "MK64F12.h"
#include "GPIO.h"

gpio_pin_control_register_t input_intr_config = GPIO_MUX1 | GPIO_PE | GPIO_PS | INTR_FALLING_EDGE;	//Configuración del GPIO

void PushButton_sw2_config(void) {
	GPIO_clock_gating(GPIO_C);								  //Activa el puerto C
	GPIO_pin_control_register(GPIO_C, 6, &input_intr_config); //Configuracion del GPIO C para el sw2
	GPIO_write_port(GPIO_C, ~(SW2));						  //Escribe un valor seguro
	GPIO_data_direction_port(GPIO_C, ~(SW2));				  //Configura el puerto del sw2 del GPIO C como Input
}

void PushButton_sw3_config(void) {
	GPIO_clock_gating(GPIO_A);								  //Activa el puerto A
	GPIO_pin_control_register(GPIO_A, 4, &input_intr_config); //Configuracion del GPIO A para el sw3
	PORTA->PCR[4] = (GPIO_MUX1 | GPIO_PE | GPIO_PS |INTR_FALLING_EDGE);
	GPIO_write_port(GPIO_A, ~(SW3));						  //Escribe un valor seguro
	GPIO_data_direction_port(GPIO_A, ~(SW3));				  //Configura el puerto del sw3 del GPIO A como Input
}

uint32_t PushButton_read(PushButton_SW_name sw) {
	uint32_t push_value = 0;
	switch (sw) {
	case PB_SW2:/** PB_SW2 is selected*/
		push_value = GPIO_read_pin(GPIO_C, SW2);
		break;
	case PB_SW3:/** PB_SW3 is selected*/
		push_value = GPIO_read_pin(GPIO_A, SW3);
		break;
	default:/**If doesn't exist the option*/
		push_value = 0x00000000;
		break;
	}
	return push_value;
}

