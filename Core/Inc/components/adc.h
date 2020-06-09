/*
 * adc.h
 *
 *  Created on: 21 de maig 2020
 *      Author: julia
 */

#ifndef INC_COMPONENTS_ADC_H_
#define INC_COMPONENTS_ADC_H_

#include "stm32f4xx_hal.h"


uint32_t GetVoltageADC(void);
uint32_t GetCurrentADC(void);

#endif /* INC_COMPONENTS_ADC_H_ */
