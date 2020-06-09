/*
 * adc.c
 *
 *  Created on: 21 de maig 2020
 *      Author: julia
 */

#include "components/adc.h"

extern ADC_HandleTypeDef hadc1;

// CELL VOLTAGE
uint32_t GetVoltageADC(void){ //funcion para obtener valor tension realmente aplicado en la celda (Vcell real)
	HAL_ADC_Start(&hadc1); //iniciamos la conversion
	HAL_ADC_PollForConversion(&hadc1,200); // esperamos que finalce la conversion
	return HAL_ADC_GetValue(&hadc1); //devuleve el valor de tension en un uint32_t
}

// CELL CURRENT
uint32_t GetCurrentADC(void){ //funcion para obtener valor de corriente que pasa por la celda (Icell)
	HAL_ADC_Start(&hadc1); //iniciamos la conversion
	HAL_ADC_PollForConversion(&hadc1,200); // esperamos que finalce la conversion
	return HAL_ADC_GetValue(&hadc1); //devuleve el valor de corriente en un uint32_t
}



