/**
  ******************************************************************************
  * @file		cyclic_voltammetry.c
  * @brief		Gestión de la voltammetría cíclica.
  * @author		Albert Álvarez Carulla
  * @copyright	Copyright 2020 Albert Álvarez Carulla. All rights reserved.
  ******************************************************************************
  */

#include "components/cyclic_voltammetry.h"
#include "components/dac.h"
#include "components/timers.h"
#include "components/adc.h"
#include "main.h"
#include "components/masb_comm_s.h"

extern TIM_HandleTypeDef htim3;
struct CV_Configuration_S cvConfiguration;


void CiclicVoltammetry(struct CV_Configuration_S cvConfiguration){

	measureIndexCV=0; //inicializamos el index de medidas

	//fijamos tension celda electroquímica a DC

	double VCELL= 1.65-cvConfiguration.eBegin/2;


	vObjetivo = cvConfiguration.eVertex1;

	MASB_COMM_i2c_sendData(VCELL); //fijamos tension Vcell a eBegin con el DAC

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET); //cerramos rele (enviamos a 1), empieza la medida

	SamplingPeriodTimerCV(); //inicializamos el timer con el periodo modificado

}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim3) { //Si transcurre el sampling period

	struct Data_S data;

	measureIndexCV++;

	data.point=measureIndexCV;
	data.timeMs = measureIndexCV*(cvConfiguration.eStep/cvConfiguration.scanRate);
	data.voltage = (1.65-voltageADC)*2; //medimos Vcell(real) con los datos del ADC
	data.current = (currentADC-1.65)*2/10e3; //medimos Icell con los datos del ADC

	MASB_COMM_S_sendData(data); //enviamos datos al host

	if (VCELL==vObjetivo){ //Si se acaba el measurement time

		if (vObjetivo==cvConfiguration.eVertex1) {
			vObjetivo = cvConfiguration.eVertex2;

		} else {
			if (vObjetivo==cvConfiguration.eVertex2){
				vObjetivo = cvConfiguration.eBegin;
				cycle++;

			} else {
				if (cycle == cvConfiguration.cycles){
					HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET); //abrimos rele, se acaba la medida
					HAL_TIM_Base_Stop_IT(&htim3); //paramos el timer

				} else{
					vObjetivo = cvConfiguration.eVertex1;
				}
			}

		}

		} else {
			if (VCELL + cvConfiguration.eStep > vObjetivo){
				VCELL= 1.65-vObjetivo/2;
				MASB_COMM_i2c_sendData(VCELL);

			} else {
				if (vObjetivo == cvConfiguration.eVertex1){
					VCELL = VCELL + cvConfiguration.eStep;
				}
				if (vObjetivo == cvConfiguration.eVertex2){
					VCELL = VCELL - cvConfiguration.eStep;
				}
				if (vObjetivo == cvConfiguration.eBegin){
					VCELL = VCELL + cvConfiguration.eStep;

				}


			}

		}
}




