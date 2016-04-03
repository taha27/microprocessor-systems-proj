/**
  ******************************************************************************
  * File Name          : segment_display.c
  * Description        : This class provides functionality for controlling the
												 7 segment display
	* Author						 : Richard Cheung, Taha Saifuddin
	* Version            : 1.0.0
	* Date							 : February 19th, 2016
  ******************************************************************************
  */
	
#include "segment_display.h"

//READ & WRITE_
//GPIO_PIN_0 
//
//Register select
//GPIO_PIN_1 
//
//GPIO_PIN_2  
//Enable
//
//DATA_PINS:
// GPIO_PIN_7 | GPIO_PIN_8  | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14
// data 0							1								2						3							4							5								6					7

/**
   * @brief A function used to initialize the LCD
	 * @retval 
   */
void LCD_init(void) 
{
	int i ;
	//reset all pins
	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_0| GPIO_PIN_1| GPIO_PIN_2 | GPIO_PIN_7 | GPIO_PIN_8  | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_RESET); 
	
	Set_RS;
	Set_RW;
	Set_En;
	
	for ( i =0; i < 65535 ; i++ ){}

	Clr_RS;
	Clr_RW;
	Clr_En;
	
	//Clear display
	LCD_clear_display();
	
	//Display & cursor HOME
	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
	LCD_set_enable();

	//Character Entry Mode
	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_9 , GPIO_PIN_SET);
	LCD_set_enable();

	// display ON
	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_10 , GPIO_PIN_SET); 
	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_8 , GPIO_PIN_RESET);
	LCD_set_enable();

	//display cursor Shift
	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_11 , GPIO_PIN_SET); 
	LCD_set_enable();

	//function set
	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_12, GPIO_PIN_SET); 
	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_10 | GPIO_PIN_9, GPIO_PIN_RESET) ;
	LCD_set_enable();
		
}

/**
   * @brief A function used to set LCD enable to HIGH->DELAY->LOW
	 * @retval 
   */
void LCD_set_enable (void)
{
	int i; 
	Set_En;
	for ( i =0; i < 300 ; i++ ){}
	Clr_En;
	for ( i =0; i < 300 ; i++ ){}
}

/**
   * @brief A function used to clear the LCD display
	 * @retval 
   */
void LCD_clear_display (void) 
{
	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
	LCD_set_enable();
}

/**
   * @brief A function used to displays to the LCD
	 * @retval
   */
void LCD_display( float f) 
{
	int i;
	int number = f*10 ;
	int position = 0;
	
	LCD_send_char(number %10) ;
	Set_RS;

	LCD_set_enable();

}

void LCD_send_char (int number) 
{
	//reset all data pin
	HAL_GPIO_WritePin (GPIOE , GPIO_PIN_7 | GPIO_PIN_8  | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 , GPIO_PIN_RESET);
	switch (number) 
	{
		case 0:
			HAL_GPIO_WritePin (GPIOE , GPIO_PIN_9| GPIO_PIN_10 , GPIO_PIN_SET);
			break;
		case 1:
			HAL_GPIO_WritePin (GPIOE , GPIO_PIN_9| GPIO_PIN_10 | GPIO_PIN_14 , GPIO_PIN_SET); 
			break;
		case 2:
			HAL_GPIO_WritePin (GPIOE , GPIO_PIN_9| GPIO_PIN_10 | GPIO_PIN_13 , GPIO_PIN_SET); 
			break;
		case 3:
			HAL_GPIO_WritePin (GPIOE , GPIO_PIN_9| GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_13, GPIO_PIN_SET); 			
			break;
		case 4:
			HAL_GPIO_WritePin (GPIOE , GPIO_PIN_9| GPIO_PIN_10 | GPIO_PIN_12, GPIO_PIN_SET); 			
			break;
		case 5:
			HAL_GPIO_WritePin (GPIOE , GPIO_PIN_9| GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_14, GPIO_PIN_SET); 			
			break;
		case 6:
			HAL_GPIO_WritePin (GPIOE , GPIO_PIN_9| GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_13  , GPIO_PIN_SET); 			
			break;
		case 7:
			HAL_GPIO_WritePin (GPIOE , GPIO_PIN_9| GPIO_PIN_10 | GPIO_PIN_12| GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_SET); 			
			break;
		case 8:
			HAL_GPIO_WritePin (GPIOE , GPIO_PIN_9| GPIO_PIN_10 | GPIO_PIN_11 , GPIO_PIN_SET); 			
			break;
		case 9:
			HAL_GPIO_WritePin (GPIOE , GPIO_PIN_9| GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_14, GPIO_PIN_SET); 			
			break;	
	}	
}

/**
   * @brief A function used to update the seven segment display with the temp value
	 * @param float f: float containing the temperature value in celcius
   */
//updates the segment display in the format of XY.Z by using the multiplexing
void update_segment_display(float f)
{	
	int number = f*10 ;
	int position = 0;
	if ( number < 1000)
	{		
		while( position != 3){
			// turn off all selected pins
			clear_select_pin();
			switch (position) 
			{
				// first digit will be set and light while the rest is off
				case 0:
					display_number( number % 10, 0);
					HAL_GPIO_WritePin( GPIOC, GPIO_PIN_0, GPIO_PIN_SET) ;
					HAL_GPIO_WritePin( GPIOC, GPIO_PIN_1 , GPIO_PIN_RESET);
					HAL_GPIO_WritePin( GPIOC, GPIO_PIN_2 , GPIO_PIN_RESET);
					HAL_GPIO_WritePin( GPIOC, GPIO_PIN_3 , GPIO_PIN_RESET);
				break; 
				// first digit will shut off and 2nd digit will light, other digits are still shut off
				case 1:
					HAL_GPIO_WritePin (GPIOC, GPIO_PIN_0 , GPIO_PIN_RESET); 
					display_number( number % 10, 1);
					HAL_GPIO_WritePin( GPIOC, GPIO_PIN_1 , GPIO_PIN_SET) ;				
					HAL_GPIO_WritePin( GPIOC, GPIO_PIN_2 , GPIO_PIN_RESET);
					HAL_GPIO_WritePin( GPIOC, GPIO_PIN_3 , GPIO_PIN_RESET);
					break;
				
				// third digit will light and others are shut off
				case 2: 
					HAL_GPIO_WritePin( GPIOC, GPIO_PIN_1 , GPIO_PIN_RESET);
					display_number( number % 10, 0);				
					HAL_GPIO_WritePin( GPIOC, GPIO_PIN_2, GPIO_PIN_SET) ;
					HAL_GPIO_WritePin (GPIOC, GPIO_PIN_0 , GPIO_PIN_RESET);
					HAL_GPIO_WritePin( GPIOC, GPIO_PIN_3 , GPIO_PIN_RESET);

				break; 
			}
			position ++; 
			number /= 10; 
		}
	}
}

// function that takes parameters number and decimal which turns on the appropriate pins to generate the right
// number for the standard 7 segment display. The decimal parameter is a flag to determine if that digit have decimal. 
//
//SEGMENT A - GPIO_PIN_0
//SEGMENT B - GPIO_PIN_1
//SEGMENT C - GPIO_PIN_2
//SEGMENT D - GPIO_PIN_3
//SEGMENT E - GPIO_PIN_4
//SEGMENT F - GPIO_PIN_5
//SEGMENT G - GPIO_PIN_6
//SEGMENT DP - GPIO_PIN_7

/**
   * @brief A function used to display a specific number using the segment pins
	 * @retval none
	 * @param decimal defines whether there is a decimal point or not
	 * @param number is the value of the digit
   */
void display_number(int number, int decimal)
{	
	clear_segment_pin();
	if (decimal) 
	{
		HAL_GPIO_WritePin (GPIOA, GPIO_PIN_7, GPIO_PIN_SET) ;
	}
	switch (number) 
	{
		case 0:
			HAL_GPIO_WritePin (GPIOA, GPIO_PIN_0 |GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3 |GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_SET);
			break;
		case 1:
			HAL_GPIO_WritePin (GPIOA , GPIO_PIN_1 | GPIO_PIN_2, GPIO_PIN_SET); 
			break;
		case 2:
			HAL_GPIO_WritePin (GPIOA , GPIO_PIN_0 | GPIO_PIN_1 |GPIO_PIN_6 | GPIO_PIN_4 | GPIO_PIN_3 , GPIO_PIN_SET); 
			break;
		case 3:
			HAL_GPIO_WritePin (GPIOA , GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6, GPIO_PIN_SET); 			
			break;
		case 4:
			HAL_GPIO_WritePin (GPIOA , GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_1 | GPIO_PIN_2, GPIO_PIN_SET); 			
			break;
		case 5:
			HAL_GPIO_WritePin (GPIOA ,GPIO_PIN_0 | GPIO_PIN_5 | GPIO_PIN_6| GPIO_PIN_2| GPIO_PIN_3, GPIO_PIN_SET); 			
			break;
		case 6:
			HAL_GPIO_WritePin (GPIOA , GPIO_PIN_0 | GPIO_PIN_5| GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_3 | GPIO_PIN_2, GPIO_PIN_SET); 			
			break;
		case 7:
			HAL_GPIO_WritePin (GPIOA ,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2, GPIO_PIN_SET); 			
			break;
		case 8:
			HAL_GPIO_WritePin (GPIOA , GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 |GPIO_PIN_4 |GPIO_PIN_5 |GPIO_PIN_6, GPIO_PIN_SET); 			
			break;
		case 9:
			HAL_GPIO_WritePin (GPIOA , GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_5 |GPIO_PIN_6, GPIO_PIN_SET); 			
			break;
	}
}

/**
   * @brief A function used to set all segments to GPIO_PIN_RESET
	 * @retval none
   */
void clear_segment_pin () 
{
	HAL_GPIO_WritePin (GPIOA , GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 |GPIO_PIN_4 |GPIO_PIN_5 |GPIO_PIN_6 |GPIO_PIN_7 , GPIO_PIN_RESET); 			
}

/**
   * @brief A function used to reset the select lines
	 * @retval none
   */
//set all select digit to GPIO_PIN_RESET
void clear_select_pin () 
{
	HAL_GPIO_WritePin (GPIOC , GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3, GPIO_PIN_RESET); 			
}
