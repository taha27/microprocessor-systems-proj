/**
  ******************************************************************************
  * File Name          : timer.c
  * Description        : This class provides functionality for initializing the
												 timer
	* Author						 : Richard Cheung, Taha Saifuddin
	* Version            : 1.0.0
	* Date							 : March 8th, 2016
  ******************************************************************************
  */

#include "timer.h"
TIM_HandleTypeDef TIM3_Handle;

/*
   * @brief A function used to configure and init the timer
   */
void TIM_Init() 
{
	//1kHz
	__TIM3_CLK_ENABLE();
	//configure TIM3_Handle
	TIM3_Handle.Instance = TIM3;
	TIM3_Handle.Init.Period= 400; //401
	TIM3_Handle.Init.Prescaler = 100; //101
	TIM3_Handle.Init.CounterMode = TIM_COUNTERMODE_UP; 
	TIM3_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	TIM3_Handle.Init.RepetitionCounter = 0;	
	
	HAL_TIM_Base_Init(&TIM3_Handle);
	HAL_TIM_Base_Start_IT(&TIM3_Handle);
	
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1);
}


