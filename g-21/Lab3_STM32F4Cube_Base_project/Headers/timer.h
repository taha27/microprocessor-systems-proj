/**
  ******************************************************************************
  * File Name          : timer.h
  * Description        : timer functions that can be used by other classes
	* Author						 : Taha Saifuddin, Richard Cheung
	* Version            : 1.0.0
	* Date							 : March 8th, 2016
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
