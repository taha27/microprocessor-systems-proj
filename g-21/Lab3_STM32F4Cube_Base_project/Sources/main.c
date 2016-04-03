/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program subroutine
	* Author						 : Ashraf Suyyagh
	* Version            : 1.0.0
	* Date							 : January 14th, 2016
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define INTERRUPT_ACTIVE_FLAG 1

/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStruct;
int EXTI0_flag_value;
int TIM3_flag_value;
int counter;
int TIM3_counter;
float threshold; 

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);

int main(void)
 {	
	//int i; 
  /* MCU Configuration----------------------------------------------------------*/
  
	HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	
  /* Initialize all configured peripherals */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	//Config LEDs
	config_animation_LEDs();
	 
	//Initialize the Accelerometer and the external interrup line 0
	configure_init_accelerometer();
	configure_interrupt_line();
	
	//Initialize the 4 digit 7segment Display
	segment_display_init();
	
	//configure tim3
	TIM_Init(); 
	
	//Initialize the kalman filter structs
	Kalmanfilter_init();
	
	while (1){
	
		//i =0;
		get_key();
		
		if (input_flag) 
		{
			if ( input_value >180) 
			{
				threshold = 0;
				input_value = 0;
				printf("%s", "Inputted value too high must be less than 180!\n");
			}
			threshold = input_value;
			input_value = 0;
			input_flag =0 ;
		}
		printf ("threshold : %f\n", threshold); 

		//If the EXTI0 callback function is called and flag is set to active, read accelerometer values
		if(EXTI0_flag_value == INTERRUPT_ACTIVE_FLAG) {
			
			//Get the accelerometer readings
			get_calibrated_acceleration();
			
			//Reset the flag
			EXTI0_flag_value = 0;
		}
		
		//If the TIM3 callback function is called and flag is set to active, update the 7segment display
		if(TIM3_flag_value == INTERRUPT_ACTIVE_FLAG) {

			//printf("Pitch angle %f\n", pitch);
			//printf("Roll angle %f\n", roll);
			update_segment_display(fabs(roll));
				//update_segment_display(1.23f);

			//compare the user defined and actual angles
			compare_user_actual_angle();
			
			//Reset the flag
			TIM3_flag_value = 0;
		}
	}
}

/** System Clock Configuration*/
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
   * @brief Sets the TIM3 interrupt flag to 1
   * @param uint16_t GPIO_Pin
   * @retval None
   */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	EXTI0_flag_value = 1;
}

/**
   * @brief Sets the TIM3 interrupt flag to 1
   * @param *htim
   * @retval None
   */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	TIM3_counter ++;
	TIM3_flag_value = 1;
	segment_display_flag = 1;
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



