/*
 * dac.c
 *
 *  Created on: 27 de maig 2020
 *      Author: julia
 */

#include "components/dac.h"
#include "components/chronoamperometry.h"
#include "components/cyclic_voltammetry.h"


void MASB_COMM_i2c_sendData(double voltageDAC) { //funcion para enviar las tensiones al dac mediante i2c

	address = 0xC0; //definimos direccion del DAC
	sendValue= round(voltageDAC*4096/4); //convertimos tension voltageDAC fijada a un valor de 2 bytes a partir de la resolucion del DAC
	//los 4 primeros bits son zeros, por lo tanto no debemos hacer nada para añadirlos
	//los doce restantes provienen de la conversion adecuada a la resolucion del DAC

	//enviamos los dos bytes uno tras otro: en este caso enviamos primero los bits mas significantes (MSB).
	i2cDataDAC[0] = (sendValue >> 8); //shifteamos los 8 bits para enviar primero MSB
	// hacemos el shift porque normalmente se guardarian los 8 bits menos significativos primero pero queremos el contrario
	i2cDataDAC[1] = sendValue;

	HAL_I2C_Master_Transmit(&hi2c1, address, i2cDataDAC, 2, 10); //transmitimos los dos bytes mediante i2c

}

