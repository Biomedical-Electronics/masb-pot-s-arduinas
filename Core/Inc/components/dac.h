/*
 * dac.h
 *
 *  Created on: 27 de maig 2020
 *      Author: julia
 */

#ifndef INC_COMPONENTS_DAC_H_
#define INC_COMPONENTS_DAC_H_

#include "stm32f4xx_hal.h"
#include "math.h" //para utilizar la funcion round

extern I2C_HandleTypeDef hi2c1;

void MASB_COMM_i2c_sendData(double voltageDAC);

uint16_t sendValue;

uint8_t i2cDataDAC[2];


#endif /* INC_COMPONENTS_DAC_H_ */
