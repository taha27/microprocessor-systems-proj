
/**
  ******************************************************************************
  * @file     : alphanumeric_pad.h
  * @brief    : This class provides functionality for configuring and 
								controlling the alphanumeric keypad
	* @author		: Richard Cheung, Taha Saifuddin
	* @version  : 2.0.0
	* @date			: March 18th, 2016
  ******************************************************************************
  */
#ifndef __ALPHANUMERIC_PAD_H
#define __ALPHANUMERIC_PAD_H

#include "stm32f4xx_hal.h"
#include "main.h"
#include "timer.h"
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX


/* Exported Variables ------------------------------------------------------- */
extern int input_value, input_flag, tilt_selection, display_mode; 
/* Exported functions ------------------------------------------------------- */
extern char get_key(void) ;
extern void set_input(char key);
/* Private functions -------------------------------------------------------- */
int get_col(void);
int get_row(void);
void debouncer(void);


#endif /* __ALPHANUMERIC_PAD_H */
