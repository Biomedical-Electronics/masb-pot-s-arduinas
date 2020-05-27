/*
 * adc.c
 *
 *  Created on: 21 de maig 2020
 *      Author: julia
 */

#include "components/adc.h"


extern ADC_HandleTypeDef hadc1;


// CELL VOLTAGE
HAL_ADC_Start(&hadc1); //iniciamos la conversion
HAL_ADC_PollForConversion(&hadc1,200); // esperamos que finalce la conversion
voltageADC = HAL_ADC_GetValue(&hadc1);


// CELL CURRENT
HAL_ADC_Start(&hadc1); //iniciamos la conversion
HAL_ADC_PollForConversion(&hadc1,200); // esperamos que finalce la conversion
currentADC = HAL_ADC_GetValue(&hadc1); // leemos el resultado de la conversion de la corriente y la guardamos en current ADC

