/**
  ******************************************************************************
  * @file      : timer.h
  * @brief     : This class provides functionality for initializing the
							 	 hardware timer
	* @author		 : Richard Cheung, Taha Saifuddin
	* @version   : 1.0.0
	* @date			 : March 8th, 2016
  ******************************************************************************
  */
#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx_hal.h"


/* Exported type      ------------------------------------------------------- */
extern TIM_HandleTypeDef TIM3_Handle;


/* Exported functions ------------------------------------------------------- */
extern void TIM_Init(void) ;


/* Private functions -------------------------------------------------------- */


#endif /* __TIMER_H */
