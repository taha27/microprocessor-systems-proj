/**
  ******************************************************************************
  * File Name          : 7seg_display.h
  * Description        : Assosciated header file for '7-seg_display.c'
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 18th, 2016
  ******************************************************************************
  */

/* Includes */
#include "stdint.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"

/* Function prototypes */
void Display(int number, int position);
void Parse_Temp(double sample, int input[]);
void Blink(void);
void Parse_Mems(int* store, float sample);
