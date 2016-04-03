/**
  ******************************************************************************
  * File Name          : MEMS_config.c
  * Description        : Initialization and configuration of MEMS sensor
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 11th, 2016
  ******************************************************************************
  */

/* Includes */
#include "MEMS_config.h"

/**
	 * @brief Configure and initialize the external MEMS accelerometer 
	 * @param void
   * @retval void
   */
void MEMS_Init(void){
	
  /* Initialize all configured peripherals */
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
void MEMS_Start_IT(void){
	
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
