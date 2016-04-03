#include "keypad.h"

//char Get_Key(void){

//	temp_col = sample_col;
//	temp_row = sample_row;
//	
//	if(temp_col == 4 && temp_row == 11)return '1';
//	else if(temp_col == 5 && temp_row == 11)return '2';
//	else if(temp_col == 6 && temp_row == 11)return '3';
//	else if(temp_col == 4 && temp_row == 12)return '4';
//	else if(temp_col == 5 && temp_row == 12)return '5';
//	else if(temp_col == 6 && temp_row == 12)return '6';
//	else if(temp_col == 4 && temp_row == 13)return '7';
//	else if(temp_col == 5 && temp_row == 13)return '8';
//	else if(temp_col == 6 && temp_row == 13)return '9';
//	else if(temp_col == 4 && temp_row == 14)return '*';
//	else if(temp_col == 5 && temp_row == 14)return '0';
//	else return '#';
//}

// row = output; column = input
int Get_Column(void){
	
	GPIO_InitTypeDef GPIOC_Init;
	GPIO_InitTypeDef GPIOB_Init;
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	// row bits set as output
	GPIOB_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOB_Init.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
	GPIOB_Init.Pull = GPIO_NOPULL;
	GPIOB_Init.Speed = GPIO_SPEED_FREQ_HIGH;	
	HAL_GPIO_Init(GPIOB, &GPIOB_Init);	
	
	// output row bits set to logic 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	
	//====================//====================//====================//
	
	// column bits set to logic 1 during idle
	GPIOC_Init.Mode = GPIO_MODE_INPUT;
	GPIOC_Init.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
	GPIOC_Init.Pull = GPIO_PULLUP;
	GPIOC_Init.Speed = GPIO_SPEED_FREQ_HIGH;	
	HAL_GPIO_Init(GPIOC, &GPIOC_Init);
	
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == GPIO_PIN_RESET 
			&& HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == GPIO_PIN_SET
			&& HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == GPIO_PIN_SET){
		
//		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
//		interrupt7 = 0;
//		// while(interrupt7 < 10);
//		if(interrupt7 > 5){
//			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == GPIO_PIN_RESET){			
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
//				return 4;
//			}
//		}
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		return 4;
	}
			
	else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == GPIO_PIN_SET 
			&& HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == GPIO_PIN_RESET
			&& HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == GPIO_PIN_SET){

//		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
//		interrupt7 = 0;
//		// while(interrupt7 < 10);
//		if(interrupt7 > 5){
//			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == GPIO_PIN_RESET){
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
//				return 5;
//			}
//		}
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
		return 5;
	}
			
	else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == GPIO_PIN_SET 
			&& HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == GPIO_PIN_SET
			&& HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == GPIO_PIN_RESET){
		
//		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
//		interrupt7 = 0;
//		// while(interrupt7 < 10);
//		if(interrupt7 > 5){
//			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == GPIO_PIN_RESET){
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
//				return 6;
//			}
//		}
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
		return 6;
	}			
	return NULL;
}

// row = input; column = output
int Get_Row(void){
	
	GPIO_InitTypeDef GPIOC_Init;
	GPIO_InitTypeDef GPIOB_Init;
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	// column bits set as output
	GPIOC_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOC_Init.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
	GPIOC_Init.Pull = GPIO_NOPULL;
	GPIOC_Init.Speed = GPIO_SPEED_FREQ_HIGH;	
	HAL_GPIO_Init(GPIOC, &GPIOC_Init);
	
	// output row bits set to logic 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
	
	//====================//====================//====================//

	// row bits set to logic 1 during idle
	GPIOB_Init.Mode = GPIO_MODE_INPUT;
	GPIOB_Init.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
	GPIOB_Init.Pull = GPIO_PULLUP;
	GPIOB_Init.Speed = GPIO_SPEED_FREQ_HIGH;	
	HAL_GPIO_Init(GPIOB, &GPIOB_Init);
	
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_RESET 
			&& HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_SET
			&& HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_SET
			&& HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_SET){
		
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
		interrupt7 = 0;
		while(interrupt7 < 50);
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_RESET){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
			return 11;
		}
	}
			
	else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_SET 
			&& HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET
			&& HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_SET
			&& HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_SET){

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
		interrupt7 = 0;
		while(interrupt7 < 50);
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
			return 12;
		}
	}
			
	else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_SET 
			&& HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_SET
			&& HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_RESET
			&& HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_SET){

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
		interrupt7 = 0;
		while(interrupt7 < 50);
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_RESET){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
			return 13;
		}
	}
			
	else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_SET 
			&& HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_SET
			&& HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_SET
			&& HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_RESET){

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
		interrupt7 = 0;
		while(interrupt7 < 50);
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_RESET){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
			return 14;
		}
	}
	return NULL;
}

