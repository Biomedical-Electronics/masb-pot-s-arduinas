/*
 * timers.c
 *
 *  Created on: 22 may. 2020
 *      Author: claud
 */

#include "components/timers.h"

#include "components/masb_comm_s.h"


extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

extern struct CA_Configuration_S caConfiguration;

//SamplingPeriodMs
void SamplingPeriodTimer(void){

	HAL_TIM_Base_DeInit(&htim3); //desinicialitzem el timer

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 8399;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = caConfiguration.samplingPeriodMs*10;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;


	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_TIM_Base_Init(&htim3); //inicialitzem

	HAL_TIM_Base_Start_IT(&htim3); //iniciem
}





