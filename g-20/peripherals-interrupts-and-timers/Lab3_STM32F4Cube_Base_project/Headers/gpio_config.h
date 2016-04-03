/**
  ******************************************************************************
  * File Name          : gpio_config.h
  * Description        : Header file for "gpio_config.c"
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 11th, 2016
  ******************************************************************************
  */

/* Includes */
#include "stm32f4xx_hal.h"

/* Extern declarations */
extern GPIO_InitTypeDef GPIOA_Init;						
extern GPIO_InitTypeDef GPIOE_Init;						
extern GPIO_InitTypeDef GPIOD_Init;

/* Function prototypes */
void GPIO_Config(void);

