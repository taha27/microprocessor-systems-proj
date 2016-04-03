/**
  ******************************************************************************
  * File Name          : interrupt_handling.c
  * Description        : Main Interrupt Service Routines- This file provides template 
	*											 for all exceptions handler and peripherals interrupt service routine
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 18th, 2016
  ******************************************************************************
  */

/* Includes */
#include "interrupt_handling.h"

/**
  * @brief  This function handles EXTI0 interrupt request
  * @param  void
  * @retval void
  */
void EXTI0_IRQHandler(void){
	
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
  * @brief  This function handles GPIO EXTI Callback
  * @param  void
  * @retval void
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	if(count > 4){
		interrupt++;
		count = 0;
	}
	count++;
}

/**
  * @brief  This function handles TIM3 interrupt request
  * @param  void
  * @retval void
  */
void TIM3_IRQHandler(void){
	
	HAL_TIM_IRQHandler(&TIM_HandleStruct);
}

/**
  * @brief  This function handles TIM3 Period-Elapsed Callback
  * @param  void
  * @retval void
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	interrupt2++;
	interrupt3++;
	interrupt4++;
	interrupt5++;
	interrupt7++;
	
	if(count2 > 10){
		interrupt6++;
		count2 = 0;
	}
	count2++;
}

