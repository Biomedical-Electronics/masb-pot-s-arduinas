/*
 * stm32main.c
 *
 *  Created on: 14 may. 2020
 *      Author: claud
 */

#include "components/stm32main.h"

#include "components/masb_comm_s.h"

struct CV_Configuration_S cvConfiguration; //on guardarem dades per configuració serie

struct CA_Configuration_S caConfiguration;


void setup(void) {

	MASB_COMM_S_waitForMessage(); //waiting the first message

}

void loop(void) {

	struct Data_S data;

	if (MASB_COMM_S_dataReceived()) {

		switch(MASB_COMM_S_command()) { //gestió del missatge rebut

			case START_CV_MEAS:

				cvConfiguration = MASB_COMM_S_getCvConfiguration();


				break;

			case START_CA_MEAS:

				caConfiguration = MASB_COMM_S_getCaConfiguration();

				break;

			case STOP_MEAS:
				break;

			default:
				data.point = 1;
				data.timeMs = 100;
				data.voltage = 0.23;
				data.current = 12.3e-6;

				MASB_COMM_S_sendData(data);

				break;
		}

		MASB_COMM_S_waitForMessage(); //un cop acabi, torno a esperar el següent
	}

}
