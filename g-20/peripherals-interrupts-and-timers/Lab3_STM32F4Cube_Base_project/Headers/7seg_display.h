/**
  ******************************************************************************
  * File Name          : 7seg_display.h
  * Description        : Header file for "7seg_display.c"
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 11th, 2016
  ******************************************************************************
  */

/* Includes */
#include "stm32f4xx_hal.h"

/* Extern declarations */
extern volatile int interrupt_2, interrupt_3;
extern int parsed[];
extern int displaying[];

/* Function prototypes */
void Display(int number, int position);
void Parse(int* store, float sample);
void Show(void);
void Show_Negative(void);
void Display_Alarm(void);
void Reset_Alarm(void);
void Move_Up(void);
void Move_Down(void);
void Show_Big(void);
