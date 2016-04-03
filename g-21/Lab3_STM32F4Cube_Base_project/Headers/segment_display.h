/**
  ******************************************************************************
  * File Name          : segment_display.h
  * Description        : Segment display functions that can be used by other classes
	* Author						 : Taha Saifuddin, Richard Cheung
	* Version            : 1.0.0
	* Date							 : February 19th, 2016
  ******************************************************************************
  */

#ifndef __SEGMENT_DISPLAY_H
#define __SEGMENT_DISPLAY_H

#include "stm32f4xx_hal.h"
#include "main.h"


/* Exported functions ------------------------------------------------------- */
extern void update_segment_display(float f);
extern void clear_segment_pin(void);
extern void clear_select_pin(void); 
extern void segment_display_init(void);

/* Private functions -------------------------------------------------------- */
void display_number(int number, int decimal);
int get_decimal_position(float f) ;
void delay(int i);

extern int segment_display_flag;
extern	int position ;

#endif /* __SEGMENT_DISPLAY_H */
