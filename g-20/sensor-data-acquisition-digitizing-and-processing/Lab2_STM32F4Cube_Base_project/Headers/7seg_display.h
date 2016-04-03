/**
  ******************************************************************************
  * File Name          : 7seg_display.h
  * Description        : Assosciated header file for '7seg_diplay.c'
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 11th, 2016
  ******************************************************************************
  */
	
/* Includes */
#include "stdint.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"

/* Function prototypes */
void Display(int number, int position);
