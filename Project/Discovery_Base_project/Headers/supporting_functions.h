/**
  ******************************************************************************
  * @file   	: supporting_functions.h
  * @brief		: User defined common functions that can be used across projects
	* @author		: Ashraf Suyyagh
	* @version	: 2.0.0
	* @date			: March 18th, 2016
  ******************************************************************************
  */
#ifndef SUPPORTING_FUNCTIONS_H
#define SUPPORTING_FUNCTIONS_H

#include "stdint.h"
#include <stdio.h>

//Add as many definitions as you see necessary. 

//NO TWO DEFINITIONS SHOULD HAVE THE SAME ERROR CODE

#define RCC_CONFIG_FAIL       ((uint16_t) 0)
#define TIM_INIT_FAIL					((uint16_t) 1)
#define ADC_INIT_FAIL 				((uint16_t) 2)
#define ADC_CH_CONFIG_FAIL		((uint16_t) 3)
#define ADC_MULTIMODE_FAIL		((uint16_t) 4)
#define EXTI_SPI1_FAIL				((uint16_t) 5)
#define ADC_START_FAIL 				((uint16_t) 6)
#define ADC_POLL_FAIL 				((uint16_t) 7)
void Error_Handler						(uint16_t error_code);

#endif
