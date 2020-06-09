/*
 * stm32main.c
 *
 *  Created on: 14 may. 2020
 *      Author: claud
 */

#include "components/stm32main.h"
#include "components/pmu.h"
#include "components/chronoamperometry.h"
#include "components/cyclic_voltammetry.h"
#include "components/masb_comm_s.h"

struct CV_Configuration_S cvConfiguration;
struct CA_Configuration_S caConfiguration;

void setup(void) {

	MASB_COMM_S_waitForMessage(); //funcion para quedar a la espera de recibir un mensaje
	StartPMU(); //llamamos funcion que habilita PMU

}

void loop(void) {

	if (MASB_COMM_S_dataReceived()) { //si recibimos un mensaje a partir del host (aplicacion Visense)

		switch(MASB_COMM_S_command()) { //gestion del mensaje recibido

			case START_CV_MEAS: //si recibimos la orden de hacer una CV

				cvConfiguration = MASB_COMM_S_getCvConfiguration(); //guardamos configuracion de los parametros
				CyclicVoltammetry(cvConfiguration); //llamamos funcion para realizar la CV

				break;

			case START_CA_MEAS: //si recibimos la orden de hacer una CA

				caConfiguration = MASB_COMM_S_getCaConfiguration(); //guardamos configuracion de los parametros
				Chronoamperometry(caConfiguration); //llamamos funcion para realizar la CA

				break;

			case STOP_MEAS: //si recibimos la orden de parar

				break; //no hacemos nada

			default: //en el caso de recibir cualquier otra cosa

				break; //no hacemos nada
		}


		MASB_COMM_S_waitForMessage(); //cuando acaba la gestion del mensaje, esperamos al siguiente
	}

}
