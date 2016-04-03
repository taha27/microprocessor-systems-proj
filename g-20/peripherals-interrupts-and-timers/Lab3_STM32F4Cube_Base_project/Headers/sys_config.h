/**
  ******************************************************************************
  * File Name          : sys_config.h
  * Description        : Header file for "sys_config.c"
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 11th, 2016
  ******************************************************************************
  */

/* Includes */
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"

/* Extern declarations */
extern TIM_Base_InitTypeDef TIM_Base_InitStruct;
extern TIM_HandleTypeDef TIM_HandleStruct;

/* Function prototypes */
void SystemClock_Config	(void);
void TIM3_Config(void);
