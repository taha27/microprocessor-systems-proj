/**
  ******************************************************************************
  * File Name          : sys_config.c
  * Description        : Configuration and implementation of system clocks and interrupt timers
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 11th, 2016
  ******************************************************************************
  */
	
/* Includes */
#include "sys_config.h"

/**
	 * @brief Configures the system clock
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
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};

  RCC_ClkInitStruct.ClockType 			= RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5)!= HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};
	
	/*Configures SysTick to provide 1ms interval interrupts. SysTick is already 
	  configured inside HAL_Init, I don't kow why the CubeMX generates this call again*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/* This function sets the source clock for the internal SysTick Timer to be the maximum,
	   in our case, HCLK is now 168MHz*/
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
	 * @brief Configures the TIM3 peripheral timer for display countdown
	 * @param void
   * @retval void
   */
void TIM3_Config(void){

	__HAL_RCC_TIM3_CLK_ENABLE();	
	TIM_Base_InitStruct.Prescaler = 1000;
	TIM_Base_InitStruct.CounterMode = TIM_COUNTERMODE_UP;
	TIM_Base_InitStruct.Period = 84;
	TIM_Base_InitStruct.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_Base_InitStruct.RepetitionCounter = 0;
	
	TIM_HandleStruct.Init = TIM_Base_InitStruct;
	TIM_HandleStruct.Instance = TIM3;
	TIM_HandleStruct.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	if(HAL_TIM_Base_Init(&TIM_HandleStruct) != HAL_OK)printf("Error initializing TIM handle\n");
	if(HAL_TIM_Base_Start_IT(&TIM_HandleStruct) != HAL_OK)printf("Error initializing TIM interrupt mode\n");
	
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1);
}
