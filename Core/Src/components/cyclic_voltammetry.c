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

void CyclicVoltammetry(struct CV_Configuration_S cvConfiguration){ //funcion que realiza la voltametria ciclica

	cycle=0; //inicializamos el numero de ciclos
	VCELL = cvConfiguration.eBegin;

	double voltageDACCV= 1.65-VCELL/2; //definimos la tension de la celda (Vcell) a partir de tension eBegin

	vObjetivo = cvConfiguration.eVertex1; //fijamos el valor de vObjetivo a tension eVertex1

	//MASB_COMM_i2c_sendData(voltageDACCV); //enviamos la tension al DAC para fijarla en la celda mediante i2c

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET); //cerramos rele, empieza la medida

	wait = FALSE; // Ponemos a False la bandera que controla los pasos a realizar cuando transcurre el periodo indicado en el timer
	uint32_t measureIndexCV=0; //inicializamos el indice de medidas

	while (cycle != cvConfiguration.cycles) { //mientras no estemos en el ultimo ciclo de medidas

			SamplingPeriodTimerCV(); //inicializamos el timer con el periodo especifico para la voltametria ciclica

			while(wait) { //cuando transcurre el periodo entre medidas

				struct Data_S data;

				data.point=measureIndexCV; //guardamos el numero de medida en el que estamos
				data.timeMs = measureIndexCV*(cvConfiguration.eStep/cvConfiguration.scanRate); //guardamos el tiempo de medida transcurrido
				data.voltage = (1.65-GetVoltageADC())*2; //medimos Vcell(real) con los datos del ADC
				data.current = (GetCurrentADC()-1.65)*2/1000; //medimos Icell con los datos del ADC

				MASB_COMM_S_sendData(data); //enviamos datos al host ( aplicacion Visense)
				measureIndexCV++;

				if (VCELL==vObjetivo){ //Si Vcell es igual a la tension objetivo definida

						if (vObjetivo==cvConfiguration.eVertex1) { //si el objectivo era eVertex1
							vObjetivo = cvConfiguration.eVertex2; //pasa a ser eVertex2

						} else if (vObjetivo==cvConfiguration.eVertex2)  { //si el objectivo era eVertex2
							vObjetivo = cvConfiguration.eBegin; //pasa a ser eBegin

						} else if (vObjetivo==cvConfiguration.eBegin) { //si el objectivo era eBegin
							vObjetivo = cvConfiguration.eVertex1; //definimos el objectivo como eVertex1
							cycle = cycle +1 ; //empezamos nuevo ciclo
						}

				} else { //Si, en cambio, Vcell no es igual a la tension objetivo definida
					if (vObjetivo == cvConfiguration.eVertex1 ){ //si el objetivo es eVertex1
						if (VCELL + cvConfiguration.eStep > vObjetivo){ //si Vcell + el step es superior al objetivo
							VCELL = vObjetivo;
							voltageDACCV= 1.65-VCELL/2; //definimos la tension de la celda (Vcell) a la tensio objetivo
							//MASB_COMM_i2c_sendData(voltageDACCV); //enviamos la tension al DAC para fijarla en la celda mediante i2c
						} else {
							VCELL = VCELL + cvConfiguration.eStep; //le sumamos eStep a Vcell
						}
					}
					if (vObjetivo == cvConfiguration.eVertex2){ //si el objetivo es eVertex2
						if (VCELL - cvConfiguration.eStep < vObjetivo){ //si Vcell - el step es inferior al objetivo
							VCELL = vObjetivo;
							voltageDACCV= 1.65-VCELL/2; //definimos la tension de la celda (Vcell) a la tensio objetivo
							//MASB_COMM_i2c_sendData(voltageDACCV); //enviamos la tension al DAC para fijarla en la celda mediante i2c
						} else {
							VCELL = VCELL - cvConfiguration.eStep;  //le restamos eStep a Vcell
						}
					}
					if (vObjetivo == cvConfiguration.eBegin ){ //si el objetivo es eBegin
						if (VCELL + cvConfiguration.eStep > vObjetivo){ //si Vcell + el step es superior al objetivo
							VCELL = vObjetivo;
							voltageDACCV= 1.65-VCELL/2; //definimos la tension de la celda (Vcell) a la tensio objetivo
							//MASB_COMM_i2c_sendData(voltageDACCV); //enviamos la tension al DAC para fijarla en la celda mediante i2c
						} else {
							VCELL = VCELL + cvConfiguration.eStep; //le sumamos eStep a Vcell
						}
					}
				}
				wait = FALSE; //desactivamos bandera para esperar hasta que pase el periodo entre medidas
			}
		}
		HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET); //abrimos rele, se acaba la medida
		HAL_TIM_Base_Stop_IT(&htim3); //paramos el timer
	}

