/*
 * pmu.c
 *
 *  Created on: May 23, 2020
 *      Author: julia
 */

#include "components/pmu.h"
#include "main.h"


void StartPMU(void) {  //funcion que inicia PMU
	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET); //habilitamos PMU
}
