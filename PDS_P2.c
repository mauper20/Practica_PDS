/*
 * unused.c
 *
 *  Created on: 3 mar 2022
 *      Author: Mauricio Peralta O , Francisco Javier
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include <stdint.h>
#include <GPIO.h>
#include <PushButton.h>
#include <RGB.h>

#define ADC_0 0
#define DELAY_SW 600000


const float g_h1[7] = { -0.1050, -0.1448, -0.1721, 0.8182, -0.1721, -0.1448, -0.1050};
const float g_h2[7] = { 0.0802, 0.0860, 0.0897, 0.0909, 0.0897, 0.0860, 0.0802};
uint8_t g_convSelec=0;


uint16_t convolutionbetween2h(uint8_t convolucionSelect, uint16_t AudIn) {
	long double convolucion = 0;
	static float reslttArray[7] = { };
	uint32_t j = 0;

	if (1 == convolucionSelect) {	//SE REALIZA CONVOLUCIÓN
		//Llenado de arreglo
		for (uint8_t i = 6; i >= 1; i--) {
			reslttArray[i] = reslttArray[i - 1];
		}
		reslttArray[0] = AudIn;
		//Operación de la convolución
		for (j = 0; j < 7; j++) {
			convolucion = (reslttArray[j] * g_h1[7 - j]);
			AudIn = (AudIn + (uint16_t) convolucion);
		}
		return AudIn;
	}
	if(2 == convolucionSelect) {	//SE REGRESA EL VALOR TAL CUAL ENTRA
		//Llenado de arreglo
		for (uint8_t i = 6; i >= 1; i--) {
			reslttArray[i] = reslttArray[i - 1];
		}
		reslttArray[0] = AudIn;
		//Operación de la convolución
		for (j = 0; j < 7; j++) {
			convolucion = (reslttArray[j] * g_h2[7 - j]);
			AudIn = (AudIn + (uint16_t) convolucion);
		}
		return AudIn;
	}
	return AudIn;
}

uint16_t ADC_return(uint8_t ADC) {
	uint16_t adcResult;
	ADC0->SC1[0] = ADC_SC1_ADCH(17); //PTE24 tarjeta, ADC0SE17 mapeado en 10001
	while ((ADC0->SC1[0] & ADC_SC1_COCO_MASK) == 0);
	adcResult = ADC0->R[0];
	return (adcResult);
}


void main(void) {

	//Configuration of clock;
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	SIM->SCGC2 = 0x1000;

	volatile uint16_t InAdc; //Variable to save the lecture of ADC
	uint32_t Salida;
	uint8_t  SalidaL;
	uint8_t  SalidaH;
	/**Variable to capture the input value*/
	uint32_t input_SW3 = 0;
	uint32_t input_SW2 = 0;
	//Valor de para el cambio de amplitud
	volatile float amplit = 1;

	//Configuración de PushButtons
	PushButton_sw3_config();
	PushButton_sw2_config();



	//Configuration ADC
	/*The configuration Register 1 (CFG1) selects the mode of operation, clock source, clock
	divide, and configuration for low power or long sample time.*/
	ADC0->CFG1 = ADC_CFG1_ADIV(0) | ADC_CFG1_ADLSMP_MASK |
	ADC_CFG1_MODE(1) | ADC_CFG1_ADICLK(0);
	/*The Status and Control Register 3 (SC3) controls the calibration, continuous convert, and
	hardware averaging functions of the ADC module.*/
	ADC0->SC3 = ADC_SC3_AVGE_MASK;
	ADC0->SC3|= ADC_SC3_AVGS(3);
	/* Configuration Register 2 (CFG2) selects the special high-speed configuration for very
	high speed conversions and selects the long sample time duration during long sample
	mode.*/
	ADC0->CFG2 = 0;
	ADC0->SC2 = 0;
	//FIN ADC configuration

	//Configuration DAC
	DAC0->C0 = 0xC0;
	DAC0->DAT[0].DATL = 0;
	DAC0->DAT[0].DATH = 0;
	// Fin configuration DAC
	for (;;) {
		InAdc = ADC_return(ADC_0);

		input_SW3 = PushButton_read(PB_SW3);
		input_SW2 = PushButton_read(PB_SW2);



		if(0==input_SW3 ){
			delay(DELAY_SW);
			amplit = amplit + 0.1;
		}
		if(input_SW2==0 ){
			delay(DELAY_SW);
			if (amplit > 0.0) {
					amplit = amplit - 0.1;
			}
		}



		//Validación de cual botón se tiene presionado ----------------------------------------------
		if (0 == input_SW2  &&  0 == input_SW3 ) { 	//SW2, baja volumen
			delay(DELAY_SW);				//Debouncer
			input_SW3 = PushButton_read(PB_SW3);//Valida si hay mas de un botón presionado
			if (0 == input_SW3) {	//SW3
				delay(DELAY_SW);			//Debouncer
				delay(DELAY_SW);
				g_convSelec++;
				if(g_convSelec==3){
					g_convSelec=0;
				}
			}
		}




		//Salida al DAC
		//Salida =  InAdc*(amplit);
		Salida =  convolutionbetween2h(g_convSelec,InAdc)*(amplit);
		SalidaL = (uint16_t) Salida & 0xFF;
		SalidaH = (uint16_t) ((Salida >> 8) & 0x0F);
		DAC0->DAT[0].DATL = SalidaL;
		DAC0->DAT[0].DATH = SalidaH;


	}
}



