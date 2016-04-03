/**
  ******************************************************************************
  * File Name          : keypad.h
  * Description        : Header file for "keypad.c"
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 11th, 2016
  ******************************************************************************
  */

/* Includes */
#include "stm32f4xx_hal.h"

/* Extern declarations */
extern GPIO_InitTypeDef GPIOC_Init;
extern GPIO_InitTypeDef GPIOB_Init;
extern volatile int interrupt_4, interrupt_5, interrupt_6;
extern int sample_col, sample_row;
extern int temp_col, temp_row;

/* Function prototypes */
int Get_Row(void);
int Get_Column(void);
void Set_Row(void);
void Set_Column(void);
char Get_Key(void);
