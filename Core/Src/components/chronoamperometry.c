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

extern TIM_HandleTypeDef htim3;
struct CA_Configuration_S caConfiguration; //preguntar si caConfiguration seran les dades q volem


void Chronoamperometry(struct CA_Configuration_S caConfiguration){

	measureIndex=0; //inicializamos el index de medidas
	TotalMeasuresNumber=(caConfiguration.measurementTime)*10e3/caConfiguration.samplingPeriodMs; //calculamos el total de medidas
	//que debemos realizar

	double voltageDAC=1.65-caConfiguration.eDC/2;

	MASB_COMM_i2c_sendData(voltageDAC); //fijamos tension Vcell a eDC con el DAC

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET); //cerramos rele (enviamos a 1), empieza la medida

	SamplingPeriodTimer(); //inicializamos el timer con el periodo modificado

}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim3) { //Si transcurre el sampling period

	struct Data_S data;

	measureIndex++;

	data.point=measureIndex;
	data.timeMs = measureIndex*caConfiguration.samplingPeriodMs;
	data.voltage = (1.65-voltageADC)*2; //medimos Vcell(real) con los datos del ADC
	data.current = (currentADC-1.65)*2/10e3; //medimos Icell con los datos del ADC

	MASB_COMM_S_sendData(data); //enviamos datos al host

	if (measureIndex==TotalMeasuresNumber){ //Si se acaba el measurement time
			HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET); //abrimos rele, se acaba la medida
			HAL_TIM_Base_Stop_IT(&htim3); //paramos el timer
		}
}
