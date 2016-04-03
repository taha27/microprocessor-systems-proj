/**
  ******************************************************************************
  * File Name          : alphanumeric_pad.h
  * Description        : 
	* Author						 : Taha Saifuddin, Richard Cheung
	* Version            : 1.0.0
	* Date							 : February 19th, 2016
  ******************************************************************************
  */

#ifndef __ALPHANUMERIC_PAD_H
#define __ALPHANUMERIC_PAD_H

#include "stm32f4xx_hal.h"
#include "main.h"
#include "timer.h"


/* Exported Variables ------------------------------------------------------- */
extern int input_value, input_flag, threshold_set_flag; 
/* Exported functions ------------------------------------------------------- */


/* Private functions -------------------------------------------------------- */
int get_col(void);
int get_row(void);
char get_key(void) ;
void debouncer(void);
void set_input(char key);


#endif /* __ALPHANUMERIC_PAD_H */
