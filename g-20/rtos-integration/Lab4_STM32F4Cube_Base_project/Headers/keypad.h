/**
  ******************************************************************************
  * File Name          : keypad.h
  * Description        : Header file for "keypad.c"
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 18th, 2016
  ******************************************************************************
  */

/* Includes */
#include "stm32f4xx_hal.h"

/* Extern declarations */
extern int interrupt7;

/* Function prototypes */
int Get_Row(void);
int Get_Column(void);
char Get_Key(void);
