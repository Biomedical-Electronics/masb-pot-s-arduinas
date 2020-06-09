/*
 * timers.c
 *
 *  Created on: 22 may. 2020
 *      Author: claud
 */

#include "components/timers.h"

#include "components/masb_comm_s.h"

extern TIM_HandleTypeDef htim3;
extern void Error_Handler(void);

struct CA_Configuration_S caConfiguration;
struct CV_Configuration_S cvConfiguration;

//timer que gestiona la CA
void SamplingPeriodTimerCA(void){ //funcion para definir perido especifico para la CA e inicializar timer

	HAL_TIM_Base_DeInit(&htim3); //desinicializamos el timer

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 8399;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = caConfiguration.samplingPeriodMs*10; //cambiamos el periodo que se habia configurado con el STM32Mx (en milisegundos)
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&htim3) != HAL_OK) //implementamos gestion de errores del timer
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

	HAL_TIM_Base_Init(&htim3); //inicializamos el timer con el periodo especifico para la CA definido

	HAL_TIM_Base_Start_IT(&htim3); //iniciamos el timer
}


//timer que gestiona la CV
void SamplingPeriodTimerCV(void){ //funcion para definir perido especifico para la CV e inicializar timer

	HAL_TIM_Base_DeInit(&htim3); //desinicializamos el timer

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 8399;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = (cvConfiguration.eStep/cvConfiguration.scanRate)*10000; //cambiamos el periodo que se habia configurado con el STM32Mx (en segundos)
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&htim3) != HAL_OK) //implementamos gestion de errores del timer
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

	HAL_TIM_Base_Init(&htim3); //inicializamos el timer con el periodo especifico para la CV definido

	HAL_TIM_Base_Start_IT(&htim3); //iniciamos el timer
}

//El callback es el mismo para los dos timers
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim3) { //Si transcurre el sampling period de CA o CV
	wait = TRUE; //activamos la bandera para que se ejecuten los pasos definidos en las funciones de CA o CV
}


