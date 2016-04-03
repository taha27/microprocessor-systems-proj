/**
  ******************************************************************************
  * File Name          : system_config.h
  * Description        : Assosciated header file for 'system_config.c'
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 11th, 2016
  ******************************************************************************
  */
	
/* Includes */
#include "stdint.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"

/* Function prototypes */
void SystemClock_Config(void);
void ADC_Config(void);
void GPIO_Config(void);
