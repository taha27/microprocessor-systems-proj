/**
  ******************************************************************************
  * @file     : segment_display.h
  * @brief    : Segment display functions that can be used by other classes
	* @author		: Richard Cheung, Taha Saifuddin
	* @version  : 2.0.0
	* @date			: March 18th, 2016
  ******************************************************************************
  */
	
#ifndef __SEGMENT_DISPLAY_H
#define __SEGMENT_DISPLAY_H

#include "stm32f4xx_hal.h"
#include "main.h"
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

/* Exported functions ------------------------------------------------------- */
extern void update_segment_display(float f);
extern void clear_segment_pin(void);
extern void clear_select_pin(void); 
extern void segment_display_init(void);
extern void flash_segment_display_alarm(void);

/* Private functions -------------------------------------------------------- */
void display_number(int number, int decimal);
int get_decimal_position(float f) ;
void delay(int i);

extern int count_for_alarm;
extern	int position ;

#endif /* __SEGMENT_DISPLAY_H */
