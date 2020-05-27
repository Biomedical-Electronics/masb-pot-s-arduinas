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

struct CV_Configuration_S cvConfiguration; //on guardarem dades per configuració serie

struct CA_Configuration_S caConfiguration;


void setup(void) {

	MASB_COMM_S_waitForMessage(); //waiting the first message
	StartPMU();

}

void loop(void) {

	struct Data_S data;

	if (MASB_COMM_S_dataReceived()) {

		switch(MASB_COMM_S_command()) { //gestió del missatge rebut

			case START_CV_MEAS:

				cvConfiguration = MASB_COMM_S_getCvConfiguration();
				CyclicVoltammetry(cvConfiguration);



				break;

			case START_CA_MEAS:

				caConfiguration = MASB_COMM_S_getCaConfiguration();
				Chronoamperometry(caConfiguration);

				break;

			case STOP_MEAS:
				break;

			default:


				break;
		}

		MASB_COMM_S_waitForMessage(); //un cop acabi, torno a esperar el següent
	}

}
