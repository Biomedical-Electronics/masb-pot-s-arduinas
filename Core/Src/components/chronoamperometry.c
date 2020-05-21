/**
  ******************************************************************************
  * @file		chronoamperometry.c
  * @brief		Gestión de la cronoamperometría.
  * @author		Albert Álvarez Carulla
  * @copyright	Copyright 2020 Albert Álvarez Carulla. All rights reserved.
  ******************************************************************************
  */

#include "components/chronoamperometry.h"

voltageDAC=1.65-caConfiguration.eDC/2;

MASB_COMM_i2c_sendData(voltageDAC); //fijamos tension Vcell a eDC con el DAC


HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET); //cerramos rele (enviamos a 1), empieza la medida

			//Si transcurre el sampling period

caConfiguration.samplingPeriodMs
caConfiguration.measurementTime


data.point = 1;
data.timeMs = 100;
data.voltage = (1.65-voltageADC)x2; //medimos Vcell(real) con los datos del ADC
data.current = (currentADC-1.65)x2/10e3; //medimos Icell con los datos del ADC

MASB_COMM_S_sendData(data); //enviamos datos al host

				//Si se acaba el measurement time

HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin_GPIO_PIN_RESET); //abrimos rele, se acaba la medida
