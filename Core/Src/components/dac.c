/*
 * dac.c
 *
 *  Created on: 27 de maig 2020
 *      Author: julia
 */

#include "components/dac.h"
#include "components/chronoamperometry.h"
#include "components/cyclic_voltammetry.h"


void MASB_COMM_i2c_sendData(double voltageDAC) {

	sendValue= round(voltageDAC*4096/4);

	i2cDataDAC[0] = (sendValue >> 8); //shifteamos los 8 bits para enviar primero MSB
	i2cDataDAC[1] = sendValue;

	HAL_I2C_Master_Transmit(&hi2c1, 0xC0, i2cDataDAC, 2, 10);

}

