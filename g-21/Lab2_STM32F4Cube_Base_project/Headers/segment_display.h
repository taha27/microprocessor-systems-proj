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

#define Set_RS HAL_GPIO_WritePin( GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
#define Clr_RS HAL_GPIO_WritePin( GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);

#define Set_RW HAL_GPIO_WritePin( GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
#define Clr_RW HAL_GPIO_WritePin( GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);

#define Set_En 	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_2, GPIO_PIN_SET);
#define Clr_En	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);


/* Exported functions ------------------------------------------------------- */
extern void update_segment_display(float f);
extern void clear_segment_pin(void);
extern void clear_select_pin(void); 
extern void LCD_init(void); 
extern void LCD_display(float f) ;

/* Private functions -------------------------------------------------------- */
void display_number(int number, int decimal);
void set_position(int pos);
void LCD_clear_display (void) ;
void LCD_send_char (int number) ;
void LCD_set_enable(void); 

#endif /* __SEGMENT_DISPLAY_H */
