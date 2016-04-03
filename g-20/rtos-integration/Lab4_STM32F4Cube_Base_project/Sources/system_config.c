/**
  ******************************************************************************
  * File Name          : system_config.c
  * Description        : Configuration and implementation of system clocks and interrupt timers
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 18th, 2016
  ******************************************************************************
  */

/* Includes */
#include "system_config.h"

/**
	 * @brief Configures the system clock
	 * @param void
   * @retval void
   */
void SystemClock_Config(void){
	
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the
     device is clocked below the maximum system frequency (see datasheet). */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

/**
	 * @brief Configures the on-chip temperature sensor and assosciated ADC channel
	 * @param void
   * @retval void
   */
void ADC_Config(void){
	
	ADC_InitTypeDef ADC1_Init;							
	ADC_ChannelConfTypeDef ADC1_Channel;
	
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

/**
	 * @brief This function configures the GPIO peripherals: it declares the necessary structures,   
	 * sets up the GPIO peripheral clock, defines the pinout number and other pertinent configurations
   * @param void
   * @retval void
   */	
void GPIO_Config(void){
	
	GPIO_InitTypeDef GPIOA_Init;						
	GPIO_InitTypeDef GPIOE_Init;
	
	/* Configure the GPIO peripheral clock */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	/* Initializes GPIO PORTS(A, E & D) and specifies pin characteristics */
	GPIOA_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOA_Init.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_8;
	GPIOA_Init.Pull = GPIO_NOPULL;
	GPIOA_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;	
	HAL_GPIO_Init(GPIOA, &GPIOA_Init);
	
	GPIOE_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOE_Init.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
	GPIOE_Init.Pull = GPIO_NOPULL;
	GPIOE_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;			
	HAL_GPIO_Init(GPIOE, &GPIOE_Init);
}	

/**
	 * @brief Configure and initialize the external MEMS accelerometer 
	 * @param void
   * @retval void
   */
void MEMS_Config(void){

	LIS3DSH_InitTypeDef LIS3DSH_InitStruct;

	LIS3DSH_InitStruct.AA_Filter_BW = LIS3DSH_AA_BW_50;
	LIS3DSH_InitStruct.Axes_Enable = LIS3DSH_XYZ_ENABLE;
	LIS3DSH_InitStruct.Continous_Update = LIS3DSH_ContinousUpdate_Disabled;
	LIS3DSH_InitStruct.Full_Scale = LIS3DSH_FULLSCALE_2;
	LIS3DSH_InitStruct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_100;
	LIS3DSH_InitStruct.Self_Test = LIS3DSH_SELFTEST_NORMAL;
	LIS3DSH_Init(&LIS3DSH_InitStruct);
}

/**
	 * @brief Enable interrupt mode on data arrival (Data Ready) for the MEMS sensor
	 * @param void
   * @retval void
   */
void MEMS_Config_IT(void){

	LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_InterruptConfigStruct;
	GPIO_InitTypeDef GPIOE_Init;

	LIS3DSH_InterruptConfigStruct.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
	LIS3DSH_InterruptConfigStruct.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	LIS3DSH_InterruptConfigStruct.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;
	LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_InterruptConfigStruct);
	
	__HAL_RCC_GPIOE_CLK_ENABLE();	
	GPIOE_Init.Mode = GPIO_MODE_IT_RISING;
	GPIOE_Init.Pin = GPIO_PIN_0;
	GPIOE_Init.Pull = GPIO_NOPULL;
	GPIOE_Init.Speed = GPIO_SPEED_FREQ_HIGH;				
	HAL_GPIO_Init(GPIOE, &GPIOE_Init);
	
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
}

/**
	 * @brief Configures the TIM3 peripheral timer for display countdown
	 * @param void
   * @retval void
   */
void TIM3_Config(void){
	
	TIM_Base_InitTypeDef TIM_Base_InitStruct;

	__HAL_RCC_TIM3_CLK_ENABLE();	
	TIM_Base_InitStruct.Prescaler = 1000;
	TIM_Base_InitStruct.CounterMode = TIM_COUNTERMODE_UP;
	TIM_Base_InitStruct.Period = 84;
	TIM_Base_InitStruct.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_Base_InitStruct.RepetitionCounter = 0;
	
	TIM_HandleStruct.Init = TIM_Base_InitStruct;
	TIM_HandleStruct.Instance = TIM3;
	TIM_HandleStruct.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	if(HAL_TIM_Base_Init(&TIM_HandleStruct) != HAL_OK) printf("Error initializing TIM handle\n");
	if(HAL_TIM_Base_Start_IT(&TIM_HandleStruct) != HAL_OK) printf("Error initializing TIM interrupt mode\n");
	
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1);
}

