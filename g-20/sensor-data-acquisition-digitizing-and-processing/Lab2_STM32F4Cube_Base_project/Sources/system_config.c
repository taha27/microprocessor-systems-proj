/**
  ******************************************************************************
  * File Name          : system_config.c
  * Description        : Configures system clock and initializes peripherals
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 11th, 2016
  ******************************************************************************
  */

/* Includes */
#include "stdint.h"
#include <stdio.h>
#include "system_config.h"
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"


/* Struct declarations for ADC handle and channel configurations */
extern ADC_InitTypeDef ADC1_Init;							
extern ADC_ChannelConfTypeDef ADC1_Channel;
extern ADC_HandleTypeDef ADC1_Handle;

/* Struct declarations for GPIO port configurations */
extern GPIO_InitTypeDef GPIOA_Init;						
extern GPIO_InitTypeDef GPIOE_Init;						
extern GPIO_InitTypeDef GPIOD_Init;

/**
	 * @brief This function configures the system clock
	 * @param void
   * @retval void
   */
void SystemClock_Config(void){

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	__PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState 			 	= RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState 		= RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM 				= 8;
	RCC_OscInitStruct.PLL.PLLN 				= 336;
	RCC_OscInitStruct.PLL.PLLP 				= RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ 				= 7;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};

	RCC_ClkInitStruct.ClockType 			= RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV2;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5)!= HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};

	/*Configures SysTick to provide 1ms interval interrupts.*/
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/* This function sets the source clock for the internal SysTick Timer to be the maximum,
	in our case, HCLK is now 168MHz*/
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
	 * @brief This function configures the GPIO peripherals: it declares the necessary structures,   
	 * sets up the GPIO peripheral clock, defines the pinout number and other pertinent configurations
   * @param void
   * @retval void
   */	
void GPIO_Config(void){
	
	/* Configure the GPIO peripheral clock */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();	
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	/* Initializes GPIO PORTS(A, E & D) and specifies pin characteristics */
	GPIOA_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOA_Init.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	GPIOA_Init.Pull = GPIO_NOPULL;
	GPIOA_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;	
	HAL_GPIO_Init(GPIOA, &GPIOA_Init);
	
	GPIOE_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOE_Init.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
	GPIOE_Init.Pull = GPIO_NOPULL;
	GPIOE_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;			
	HAL_GPIO_Init(GPIOE, &GPIOE_Init);
	
	GPIOD_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOD_Init.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIOD_Init.Pull = GPIO_NOPULL;
	GPIOD_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;	
	HAL_GPIO_Init(GPIOD, &GPIOD_Init);
}	

/**
	 * @brief This function configures the ADC peripherals: it declares the necessary structures
   * and handles, configures the pertinent channel of interest (channel 16 for temperature) 
	 * and sets up clock and the operating parameters for scanning and polling
   * @param void
   * @retval void
   */
void ADC_Config(void){
	
	/* Configure the ADC peripheral clock */
	__HAL_RCC_ADC1_CLK_ENABLE();
	
	/* Initialize ADC handle, init structure and channel configurations (selects channel 16) */
	ADC1_Handle.Instance = ADC1;
	ADC1_Handle.Init = ADC1_Init;
	
	ADC1_Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	ADC1_Init.Resolution = ADC_RESOLUTION_12B;
	ADC1_Init.ScanConvMode = DISABLE;
	ADC1_Init.ContinuousConvMode = ENABLE;
	ADC1_Init.DiscontinuousConvMode = DISABLE;
	ADC1_Init.NbrOfDiscConversion = 0;
	ADC1_Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADC1_Init.ExternalTrigConv = ADC_SOFTWARE_START;
	ADC1_Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC1_Init.NbrOfConversion = 1;
	ADC1_Init.DMAContinuousRequests = DISABLE;
	ADC1_Init.EOCSelection = ADC_EOC_SEQ_CONV;
	
	ADC1_Channel.Channel = ADC_CHANNEL_16;
	ADC1_Channel.Rank = 1;
	ADC1_Channel.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	ADC1_Channel.Offset = 0;
	
	/* Use of supporting functions to check proper boot-up of peripherals, report on error */
	if(HAL_ADC_Init(&ADC1_Handle) != HAL_OK){
		// printf("ADC initialization failed!\n");
		Error_Handler(ADC_INIT_FAIL);
	}		
	if(HAL_ADC_ConfigChannel(&ADC1_Handle, &ADC1_Channel) != HAL_OK){
		// printf("Channel configuration failed!\n");
		Error_Handler(ADC_CH_CONFIG_FAIL);
	}
}
