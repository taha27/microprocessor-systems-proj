/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program subroutine
	* Author						 : Aditya Saha & Habib Ahmed (Original template by Ashraf Suyyagh)
	* Version            : 1.0.0
	* Date							 : January 14th, 2016
  ******************************************************************************
  */
	
/* Includes */
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "sys_config.h"
#include "MEMS_config.h"
#include "7seg_display.h"
#include "gpio_config.h"
#include "keypad.h"
#include "get_readings.h"
#define DESIRED_ANGLE 40.0

typedef int bool;
#define true 1
#define false 0

/* BLUE LED = DECREASE ANGLE */
/* ORANGE LED = INCREASE ANGLE */

/* Function prototypes */
void arm_abs_f32(float32_t *pSrc, float32_t *pDst, uint32_t blockSize);

/* Global declarations */
LIS3DSH_InitTypeDef LIS3DSH_InitStruct;
LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_InterruptConfigStruct;
GPIO_InitTypeDef GPIOE_Init;
TIM_Base_InitTypeDef TIM_Base_InitStruct;
TIM_HandleTypeDef TIM_HandleStruct;

GPIO_InitTypeDef GPIOA_Init;						
GPIO_InitTypeDef GPIOE_Init;						
GPIO_InitTypeDef GPIOD_Init;
GPIO_InitTypeDef GPIOC_Init;
GPIO_InitTypeDef GPIOB_Init;

volatile int interrupt, interrupt_2, interrupt_3, interrupt_4, interrupt_5, interrupt_6;
int counter;

float read_acc[] = {0, 0, 0};
int parsed[] = {0, 0, 0, 0, 0};
int displaying[] = {0, 0, 0, 0, 0};

int sample_col, sample_row;
int temp_col, temp_row;

float output_x[] = {0};
float output_y[] = {0};
float output_z[] = {0};

kalman_state kstate_x;
kalman_state kstate_y;
kalman_state kstate_z;

float den_pitch, den_roll;
float pitch, roll;

float diff_in, diff_out;

int main(void){		
		
	//===========================================
//	bool flag = true;
//	int count = 0;
//	int sw = 0;
//	interrupt_4 = 0;	
//	
//	__HAL_RCC_GPIOC_CLK_ENABLE();
//	__HAL_RCC_GPIOB_CLK_ENABLE();
//	
//	// row bits set as output
//	GPIOB_Init.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIOB_Init.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
//	GPIOB_Init.Pull = GPIO_NOPULL;
//	GPIOB_Init.Speed = GPIO_SPEED_FREQ_HIGH;	
//	HAL_GPIO_Init(GPIOB, &GPIOB_Init);	
//	
//	// output row bits set to logic 0
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
//		
//	// column bits set to logic 1 during idle
//	GPIOC_Init.Mode = GPIO_MODE_INPUT;
//	GPIOC_Init.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
//	GPIOC_Init.Pull = GPIO_PULLUP;
//	GPIOC_Init.Speed = GPIO_SPEED_FREQ_HIGH;	
//	HAL_GPIO_Init(GPIOC, &GPIOC_Init);
	
	
	//===========================================
	
			
//	char char_1 = '\0';
//	char char_2 = '\0';
//	char char_3 = '\0';
//	char char_4 = '\0';
	char char_store[] = {'\0', '\0', '\0', '\0'};
	int i = 0;
	
	int lock = 0;
	float temp;
	
	
	/* MCU Configuration */
  HAL_Init();

  /* Configure the system clock, peripherals and timers */
  SystemClock_Config();
	MEMS_Init();
	MEMS_Start_IT();
	TIM3_Config();	
	GPIO_Config();
	
	Reset(&kstate_x);	
	Reset(&kstate_y);
	Reset(&kstate_z);
	
	counter = 0;
	interrupt = 0;
	interrupt_5 = 0;

	
	while (1){
		
		Get_Sensor_Data(DESIRED_ANGLE);		
		// if((roll - alarm) <= 4)Display_Alarm();
		// Reset_Alarm();
		
//		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == GPIO_PIN_RESET){
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
//				interrupt_4 = 0;
//				while(interrupt_4 < 150);
//				if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == GPIO_PIN_RESET){
//					printf("1\n");
//					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
//				}
//		}		
		
//		sample_col = Get_Column();
//		sample_row = Get_Row();
//		if(sample_col != NULL && sample_row != NULL){
//			printf("gets here\n");
//			char_1 = Get_Key();
//			printf("%c\n", char_1);
//		}
				
				
		// if(lock == 0){
							
//			if(i == 0){
//				sample_col = Get_Column();
//				sample_row = Get_Row();
//				if(sample_col != NULL && sample_row != NULL && char_store[i] == '\0'){
//					char_store[i] = Get_Key();
//					printf("char_[%d] = %c entered\n", i, char_store[i]);
//					i++;
//					// printf("going here %d\n", i);
//				}
//				// while(interrupt_5 < 50);
//		}
//			
//		if(i == 1){
//			printf("inside1\n");
//			if(char_store[i-1] != '#'){			
//				sample_col = Get_Column();
//				sample_row = Get_Row();
//				// printf("inside2\n");
//				if(sample_col != NULL && sample_row != NULL && char_store[i] == '\0'){
//					// printf("inside3\n");
//					char_store[i] = Get_Key();
//					printf("char_[%d] = %c entered\n", i, char_store[i]);
//					i++;
//					printf("going here %d\n", i);
//				}
//			} else{
//					i = 0;
//					char_store[i] = '\0';
//			}
//			// while(interrupt_5 < 100);
//		}
//			
//		if(i == 2){
//			if(char_store[i-1] != '#'){
//				sample_col = Get_Column();
//				sample_row = Get_Row();
//				if(sample_col != NULL && sample_row != NULL && char_store[i] == '\0'){
//					char_store[i] = Get_Key();
//					printf("char_[%d] = %c entered\n", i, char_store[i]);
//					i++;
//					printf("going here %d\n", i);
//				}
//			} else{
//	//				temp = char_1 - '0';
//					// lock = 1;
//					// Get_Sensor_Data(temp);
//				printf("# entered\n");
//			}
//			// while(interrupt_5 < 150);
//		}
//			
//		else{
//			if(char_store[i-1] != '#'){
//				sample_col = Get_Column();
//				sample_row = Get_Row();
//				if(sample_col != NULL && sample_row != NULL && char_store[i] == '\0'){
//					char_store[i] = Get_Key();
//					printf("char_[%d] = %c entered\n", i, char_store[i]);
//				}
//			} else{
//	//				temp = (char_1 - '0') * 10;
//	//				temp = temp + (char_2 - '0');	
//					// lock = 1;		
//					// Get_Sensor_Data(temp);
//				printf("# entered\n");
//			}
//			// while(interrupt_5 < 200);
//			
//			
//			if(char_store[0] != '#'){
//				char_store[1] = '\0';
//				char_store[2] = '\0';
//				char_store[3] = '\0';
//				i = 0;
//			} else{
//	//				temp = (char_1 - '0') * 100;
//	//				temp = temp + ((char_2 - '0') * 10);
//	//				temp = temp + (char_3 - '0');
//					// lock = 1;
//					// Get_Sensor_Data(temp);
//				printf("# entered\n");
//			} 
//		// }
//		}
	}
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line){
}
#endif

