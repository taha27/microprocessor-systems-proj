/**
  ******************************************************************************
  * File Name          : alphanumeric_pad.c
  * Description        : This class provides functionality for configuring and 
												 controlling the alphanumeric keypad
	* Author						 : Richard Cheung, Taha Saifuddin
	* Version            : 1.0.0
	* Date							 : March 10th, 2016
  ******************************************************************************
  */
	
#include "alphanumeric_pad.h"


int column;
int row;
int keypad_state;
int input_value; 
int input_flag; 
int threshold_set_flag;

char key = ' ';
const int IDLE = 0;
const int DEBOUNCING = 1;
const int PRESSED = 2;
const int RELEASED = 3;
char keys[4][4] = 
{  
 {'1', '2', '3', 'A'},
 {'4', '5', '6', 'B'},
 {'7', '8', '9', 'C'},
 {'*', '0', '#', 'D'},
};
/**
   * @brief function concat int with the form of XY
	 * @param takes parameter x and y to concat both number
*/
int int_concat(int x, int y){
	return 10*x + y;
}

/**
   * @brief function to set the input value with the keys pressed
	 * @param takes parameter key from get_key in order to do map the right char
   */
void set_input(char key)
{
	switch (key) 
	{
		case '0':
			input_value = int_concat(input_value,0);
			break;
		case '1':
			input_value = int_concat(input_value,1);
			break;
		case '2':
			input_value = int_concat(input_value,2);
			break;
		case '3':
			input_value = int_concat(input_value,3);
			break;
		case '4':
			input_value = int_concat(input_value,4);
			break;
		case '5':
			input_value = int_concat(input_value,5);
			break;
		case '6':
			input_value = int_concat(input_value,6);
			break;
		case '7':
			input_value = int_concat(input_value,7);
			break;
		case '8':
			input_value = int_concat(input_value,8);
			break;
		case '9':
			input_value = int_concat(input_value,9);
			break;
		default:
			input_flag = 1;
			threshold_set_flag = 1;
			break;
	}
}

/**
   * @brief function to get the key pressed
	 * @retval key: returns the key associated with the keypad mapping position
   */
char get_key() 
{
	column = get_col();
	row = get_row();
	
	debouncer();
	//printf("%c %d%d\n", key, row, column);
	return key;
}
/**
   * @brief function to handle debouncer
	 * @param 
   */
void debouncer()
{
	switch(keypad_state){
		case IDLE:
			TIM3_counter = 0;
			if(column != -1 && row != -1){
				keypad_state = DEBOUNCING;
			}
			break;
		case DEBOUNCING:
			TIM3_counter = 0;
			if(TIM3_counter % 1000 == 0){
				key = keys[row][column];
				set_input(key);
				keypad_state = PRESSED;
			}
			break;
		case PRESSED:
			if(column == -1 || row == -1){
				keypad_state = RELEASED;
			}
			break;
		case RELEASED:
			TIM3_counter = 0;
			if(TIM3_counter % 1000 == 0){
				keypad_state = IDLE;
				key = ' ';
			}
			break;
	
	}
}


/**
   * @brief function to determine the column that the key is pressed, where PIN0,PIN1,PIN2,PIN3
						are respectively column 1,2,3,4
	 * @param 
   */
int get_col() 
{
	//first 4 pins of the alphanumeric set to INPUT
	GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_1 | GPIO_PIN_5 | GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pull = GPIO_PULLUP; 
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	//last 4 pins of the alphanumeric set to OUTPUT
	GPIO_InitStruct.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pull = GPIO_NOPULL; 
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
//		printf ("%d ", HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3));
//		printf ("%d ", HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1));
//		printf ("%d ", HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5));
//		printf ("%d \n ", HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6));
//	
	
	if ( HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3) == GPIO_PIN_RESET ) {
		return 0;
	}
	else if ( HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1) == GPIO_PIN_RESET ) {
		return 1;
	}
	else if ( HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5) == GPIO_PIN_RESET ) {
		return 2;
	}
	else if ( HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6) == GPIO_PIN_RESET ) {
		return 3;
	}
	else {
		return -1;
	}
}
/**
   * @brief function to determine the row that the key is pressed, where PIN4,PIN5,PIN6,PIN7
						are respectively row 1,2,3,4
	 * @param 
   */
int get_row() 
{
	//first 4 pins of the alphanumeric set to OUTPUT
	GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_1 | GPIO_PIN_5 | GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pull = GPIO_NOPULL; 
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	//last 4 pins of the alphanumeric set to INPUT
	GPIO_InitStruct.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pull = GPIO_PULLUP; 
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
//		printf ("%d ", HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_12));
//		printf ("%d ", HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13));
//		printf ("%d ", HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14));
//		printf ("%d \n ", HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15));
	
	if ( HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_12) == GPIO_PIN_RESET ) {
		return 0;
	}
	else if ( HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13) == GPIO_PIN_RESET ) {
		return 1;
	}
	else if ( HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14) == GPIO_PIN_RESET ) {
		return 2;
	}
	else if ( HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15) == GPIO_PIN_RESET ) {
		return 3;
	}
	else {
		return -1;
	}
}
