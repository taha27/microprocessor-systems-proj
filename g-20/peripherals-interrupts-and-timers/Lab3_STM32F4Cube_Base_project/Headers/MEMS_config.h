/**
  ******************************************************************************
  * File Name          : MEMS_config.h
  * Description        : Header file for "MEMS_config.c"
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 11th, 2016
  ******************************************************************************
  */

/* Includes */
#include "stm32f4xx_hal.h"
#include "lis3dsh.h"

/* Extern declarations */
extern LIS3DSH_InitTypeDef LIS3DSH_InitStruct;
extern LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_InterruptConfigStruct;
extern GPIO_InitTypeDef GPIOE_Init;

/* Function prototypes */
void MEMS_Init(void);
void MEMS_Start_IT(void);

