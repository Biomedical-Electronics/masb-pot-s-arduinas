/**
  ******************************************************************************
  * @file		chronoamperometry.c
  * @brief		Gestión de la cronoamperometría.
  * @author		Albert Álvarez Carulla
  * @copyright	Copyright 2020 Albert Álvarez Carulla. All rights reserved.
  ******************************************************************************
  */

#include "components/chronoamperometry.h"
#include "main.h"
#include "components/masb_comm_s.h"
#include "components/dac.h"
#include "components/timers.h"
#include "components/adc.h"

extern TIM_HandleTypeDef htim3;
struct CA_Configuration_S caConfiguration;

void Chronoamperometry(struct CA_Configuration_S caConfiguration){ //funcion que realiza la cronoamperometria

	TotalMeasuresNumber=(caConfiguration.measurementTime)*10e3/caConfiguration.samplingPeriodMs;
	//calculamos el total de medidas que debemos realizar

	double voltageDAC=1.65-caConfiguration.eDC/2; //definimos la tension de la celda (Vcell) a partir de tension eDC

	//MASB_COMM_i2c_sendData(voltageDAC); //enviamos la tension al DAC para fijarla en la celda mediante i2c

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET); //cerramos rele, empieza la medida

	wait = FALSE; // Ponemos a False la bandera que controla los pasos a realizar cuando transcurre el periodo indicado en el timer

	uint32_t measureIndex = 0; //inicializamos el indice de medidas

	while (measureIndex != TotalMeasuresNumber) { //mientras no se hayan hecho todas las medidas

		SamplingPeriodTimerCA(); //inicializamos el timer con el periodo especifico para la cronoamperometria

		while(wait == TRUE) { //cuando transcurre el periodo entre medidas

			struct Data_S data;
			measureIndex++;

			data.point=measureIndex; //guardamos el numero de medida en el que estamos
			data.timeMs = measureIndex*caConfiguration.samplingPeriodMs; //guardamos el tiempo de medida transcurrido
			data.voltage = (1.65-GetVoltageADC())*2; //medimos Vcell(real) con los datos del ADC
			data.current = (GetCurrentADC()-1.65)*2/10e3; //medimos Icell con los datos del ADC

			MASB_COMM_S_sendData(data); //enviamos datos al host (aplicacion Visense)

			wait = FALSE; //desactivamos bandera para esperar hasta que pase el periodo entre medidas
		}
	}

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET); //abrimos rele, se acaba la medida
	HAL_TIM_Base_Stop_IT(&htim3); //paramos el timer
}



