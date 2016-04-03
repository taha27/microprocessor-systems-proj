/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : A program which showcases ADC and TIM3 under the new firmware
	                       The ADC
	* Author						 : Aditya Saha & Habib Ahmed (Original template by Ashraf Suyyagh)
	* Version            : 1.0.0
	* Date							 : January 14th, 2016
  ******************************************************************************
  */
	
/* Includes */
#include "stdint.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "kalman_filter.h"
#include "system_config.h"
#include "7seg_display.h"

/* Struct declarations for ADC handle and channel configurations */
ADC_InitTypeDef ADC1_Init;							
ADC_ChannelConfTypeDef ADC1_Channel;
ADC_HandleTypeDef ADC1_Handle;

/* Struct declarations for GPIO port configurations */
GPIO_InitTypeDef GPIOA_Init;						
GPIO_InitTypeDef GPIOE_Init;						
GPIO_InitTypeDef GPIOD_Init;

int ticks, ticks_2 = 0;

int main(void){
	
	/* Local variable declarations */
	double temp, sample;
	double output[1] = {0};
	int store[4] = {0, 0, 0, 0};
	int temp_store[4] = {0, 0, 0, 0};
	int i, sample2;
	int counter, counter2, counter3;	
	kalman_state kstate;

	/* MCU configuration */
	/* Reset of all peripherals, initializes the flash interface and the systick */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Configure the ADC and GPIO peripherals */
	ADC_Config();
	GPIO_Config();		

	/* Initializes Kalman state struct and sets the loop counter to impose delays */
	Reset(&kstate);	
	
	counter = 1;			// loop counter to set time delay for all 3 digits display
	counter2 = 0;			// loop counter to set time delay for only 1 digit display
	counter3 = 0;			// loop counter to set time delay for the alarm signal LEDs
	
	while (1){
		
		/* Wait for SysTick interrupt before polling for ADC conversion */
		while(!ticks);
		ticks = 0;
		
		/* Start conversion and poll for completion before fetching ADC output data */
		HAL_ADC_Start(&ADC1_Handle);
		if(HAL_ADC_PollForConversion(&ADC1_Handle, 1000000) == HAL_OK){
			temp = HAL_ADC_GetValue(&ADC1_Handle);
			temp = (temp * 3000) / 4096;
			temp = ((temp - 760) / 2.5) + 25;	
			
			/* Pass ADC output data through Kalman filter to discard noise and apply formula found 
			 * in the datasheel to do the necessary conversion to degree Celsius */			
			if(!Kalmanfilter_C(temp, output, &kstate)){			
				// printf("Raw temp= %f, Filter out temp= %f\n", temp, output[0]);
				// printf("%f\n", output[0]);
				sample = output[0];
				
				/* Extract separate digits from decimal valued ADC output(to be used for 
				 * output of digits to the 7 segment display */ 
				sample2 = sample * 10;
				i = 0;
				while(i < 2){
					store[i] = sample2 % 10;
					sample2 /= 10;
					i++;
				}
				store[2] = sample / 10;
				// printf("temp = %f, counter = %d\n", sample, counter);

				store[3] = store[2];
				store[2] = store[1];
				store[1] = 10;
				// printf("%d %d %d %d\n", store[3], store[2], store[1], store[0]);
				
				/* Start loop counter to impose delay on updating the segment display without 
				 * interrupting temperature polling or any other services */
				if(counter == 1){
					temp_store[3] = store[3];
					temp_store[2] = store[2];
					temp_store[1] = store[1];
					temp_store[0] = store[0];
					printf("%d %d %d %d\n", temp_store[3], temp_store[2], temp_store[1], temp_store[0]);
				}		
				
				/* Start loop counter to impose delay on the LED lights designated for the overheat alarm 
				 * NOTE: alarm set to go off for temperature values over 54 degree Celsius */
				if(temp_store[3] == 5 && temp_store[2] > 0){
					if(counter3 <= 5){
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
					}
					else if(counter3 <= 10){
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
					}
					else if(counter3 <= 15){
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
					}
					else{
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
					}
				}
				
				/* Start loop counter to impose delay on per segment per digit LED lights.
				 * The LED digits are displayed incrementally (swept across the segment display)
				 * hence this loop counter allows setting reasonable amount of delay to setup
				 * a smooth transitioning display with less recognizable flickers */
				if(counter2 <= 2){
					Display(temp_store[3], 4);
				}
				
				else if(counter2 <= 4){
					Display(temp_store[2], 3);
				}
				
				else if(counter2 <= 6){
					Display(temp_store[0], 1);
				}
				
				else{
					Display(temp_store[1], 2);						
				}

				/* Reset loop counter after 90 cycles */
				if(counter > 1000){
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
					counter = 0;					
				}
				
				/* Reset loop counter after 4 cycles */
				if(counter2 > 6){
					counter2 = 0;
				}
				
				/* Reset loop counter after 20 cycles */
				if(counter3 >= 20){
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
					counter3 = 0;
				}
			}
			
			/* If Kalman function outputs high return value to signify overflow or NaN,
			 * report to debugger */
			else{
				printf("Error in Kalman function output!\n");
			}
		}
		
		/* Increment loop counter variable to update before next ADC polling event */
		counter++;
		counter2++;
		counter3++;
	}
}

/**
   * @brief A function used to setup the ADC to sample Channel 16
   * @retval None
   */

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
