/**
  ******************************************************************************
  * @file     : segment_display.c
  * @brief    : This class provides functionality for controlling the
								7 segment display
	* @author		: Richard Cheung, Taha Saifuddin
	* @version  : 2.0.0
	* @date			: March 18th, 2016
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "segment_display.h"

/* Defines ------------------------------------------------------------------ */


/* Variables -----------------------------------------------------------------*/
int segment_display_flag;
int position;
int number;
int update_flag ;
int decimal_position_one, decimal_position_ten;
int count_for_alarm;

/**
   * @brief A function used to update the seven segment display with the temp value
	 * @param float f: float containing the temperature value in celcius
   */
//updates the segment display in the format of XY.Z by using the multiplexing
void update_segment_display(float f)
{	
	clear_select_pin(); 
	clear_segment_pin();
	if (!update_flag) {
		if (f < 10){
			decimal_position_ten =0;
			decimal_position_one =1;
			number = f*100;
		}
		else if (f < 100) {
			decimal_position_ten =1;
			decimal_position_one =0;
			number = f*10;
		}
		else{
			decimal_position_ten =0;
			decimal_position_one =0;
			number= f;
		}
	}
		// turn off all selected pins
		clear_select_pin();
		clear_segment_pin();
		switch (position) 
		{
			// first digit will be set and light while the rest is off
			case 0:
				display_number( number % 10, 0);
				HAL_GPIO_WritePin( GPIOE, GPIO_PIN_3, GPIO_PIN_SET) ;
				position ++; 
						update_flag = 1;

				break; 
			// first digit will shut off and 2nd digit will light, other digits are still shut off
			case 1:
				display_number( number % 10, decimal_position_ten);
				HAL_GPIO_WritePin( GPIOE, GPIO_PIN_2 , GPIO_PIN_SET) ;	
				position ++; 
						update_flag = 1;

				break;
			
			// third digit will light and others are shut off
			case 2: 
				display_number( number % 10, decimal_position_one);				
				HAL_GPIO_WritePin( GPIOE, GPIO_PIN_1, GPIO_PIN_SET) ;
				position ++; 
				update_flag = 1;
				break;
			
			case 3:
				position ++;
				update_flag = 1;
				break;
		}
		number /= 10;
		if (position == 4) {
			clear_select_pin(); 
			clear_segment_pin();
			HAL_GPIO_WritePin( GPIOE, GPIO_PIN_4, GPIO_PIN_SET) ;
			HAL_GPIO_WritePin( GPIOC, GPIO_PIN_3, GPIO_PIN_SET) ;
			position = 0;
			update_flag = 0;
		}
	}

//void delay(int i){
//	int j;
//	for (j = 0; j<i ; j++) {}
//}

//int get_decimal_position(float f) 
//{
//	int i;
//	char c[10];
//	sprintf(c, "%f", f);
//	for (  i = 0 ; i < 4; i++) {
//		if (c[i] == '.') {
//			 return i; 
//		} 
//	}
//	return 0; 
//}	

/**
   * @brief A function used to init the segment display
	 * @retval none
   */
void segment_display_init () 
{
		//Configure GPIOE for the 4 select lines
	GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pull = GPIO_NOPULL; 
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	//Configure GPIOC for the segments
	GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pull = GPIO_NOPULL; 
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

// function that takes parameters number and decimal which turns on the appropriate pins to generate the right
// number for the standard 7 segment display. The decimal parameter is a flag to determine if that digit have decimal. 
//
//SEGMENT A - GPIO_PIN_1
//SEGMENT B - GPIO_PIN_2
//SEGMENT C - GPIO_PIN_3
//SEGMENT D - GPIO_PIN_4
//SEGMENT E - GPIO_PIN_5
//SEGMENT F - GPIO_PIN_10
//SEGMENT G - GPIO_PIN_11
//SEGMENT DP - GPIO_PIN_12

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
		HAL_GPIO_WritePin (GPIOC, GPIO_PIN_12, GPIO_PIN_SET) ;
	}
	switch (number) 
	{
		case 0:
			HAL_GPIO_WritePin (GPIOC, GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3 |GPIO_PIN_4 |GPIO_PIN_5 | GPIO_PIN_10, GPIO_PIN_SET);
			break;
		case 1:
			HAL_GPIO_WritePin (GPIOC , GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_SET); 
			break;
		case 2:
			HAL_GPIO_WritePin (GPIOC , GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_11 | GPIO_PIN_5 | GPIO_PIN_4 , GPIO_PIN_SET); 
			break;
		case 3:
			HAL_GPIO_WritePin (GPIOC , GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_11, GPIO_PIN_SET); 			
			break;
		case 4:
			HAL_GPIO_WritePin (GPIOC , GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_SET); 			
			break;
		case 5:
			HAL_GPIO_WritePin (GPIOC ,GPIO_PIN_1 | GPIO_PIN_10 | GPIO_PIN_11| GPIO_PIN_3| GPIO_PIN_4, GPIO_PIN_SET); 			
			break;
		case 6:
			HAL_GPIO_WritePin (GPIOC , GPIO_PIN_1 |GPIO_PIN_3 |GPIO_PIN_4| GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_10, GPIO_PIN_SET); 			
			break;
		case 7:
			HAL_GPIO_WritePin (GPIOC ,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_SET); 			
			break;
		case 8:
			HAL_GPIO_WritePin (GPIOC , GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3| GPIO_PIN_4 |GPIO_PIN_5 |GPIO_PIN_10 |GPIO_PIN_11, GPIO_PIN_SET); 			
			break;
		case 9:
			HAL_GPIO_WritePin (GPIOC , GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |GPIO_PIN_10 |GPIO_PIN_11, GPIO_PIN_SET); 			
			break;
	}
}

/**
   * @brief A function used to set all segments to GPIO_PIN_RESET
	 * @retval none
   */
void clear_segment_pin (void) 
{
	HAL_GPIO_WritePin (GPIOC , GPIO_PIN_12 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 |GPIO_PIN_4 |GPIO_PIN_5 |GPIO_PIN_11 |GPIO_PIN_10 , GPIO_PIN_RESET); 			
}

/**
   * @brief A function used to reset the select lines
	 * @retval none
   */
//set all select digit to GPIO_PIN_RESET
void clear_select_pin (void) 
{
	HAL_GPIO_WritePin (GPIOE , GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3| GPIO_PIN_4, GPIO_PIN_RESET); 			
}

/**
   * @brief A function used to flash the 7 segment display as overheat alarm
	 * @retval none
   */
void flash_segment_display_alarm()
{	
	//Clear the 7segment display
	if(count_for_alarm >= 350 && count_for_alarm < 499) {
		is_alarm_on = 1;
		clear_segment_pin();
		clear_select_pin();
	} else is_alarm_on = 0;
	
	//Reset the counter to 0
	if(count_for_alarm >= 500){
		is_alarm_on = 0;
		count_for_alarm = 0;
	}
}
