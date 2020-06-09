/*
 * timers.h
 *
 *  Created on: 22 may. 2020
 *      Author: claud
 */

#ifndef INC_COMPONENTS_TIMERS_H_
#define INC_COMPONENTS_TIMERS_H_

#include "stm32f4xx_hal.h"
#include "components/masb_comm_s.h"

//Prototype functions

void SamplingPeriodTimerCA(void);
void SamplingPeriodTimerCV(void);

_Bool wait;


#endif /* INC_COMPONENTS_TIMERS_H_ */
