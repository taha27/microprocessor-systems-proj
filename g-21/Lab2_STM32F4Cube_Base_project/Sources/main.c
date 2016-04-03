/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : A program which showcases ADC and TIM3 under the new firmware
	                       The ADC
	* Author						 : Richard Cheung, Taha Saifuddin
	* Version            : 1.0.0
	* Date							 : February 19th, 2016
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "init.c"
#include "main.h"


/* Constants -----------------------------------------------------------------*/
#define ALARM_PERIOD	500
//#define VOLTAGE_CONVERSION(x)	(float)(x*(3.0/4096))
#define OVERHEAT_TEMP	(float)50.0
#define ALARM_RESET -1	

/* Private variables ---------------------------------------------------------*/
ADC_InitTypeDef ADC_InitStruct;
ADC_ChannelConfTypeDef ADC_ChannelStruct;
GPIO_InitTypeDef GPIO_InitStruct;
kalman_state current_kstate;

/* Global variables ----------------------------------------------------------*/
ADC_HandleTypeDef ADC1_Handle;
int loop_count_gbl;
int ticks = 0;

//int display_ticks; 

/* Private function implementations ------------------------------------------*/
int main(void)
{
	
	//testing adc
	int adc_started = 0;
	int adc_valid = 0;
 	
	//Variables to store temperature
	float temperature = 0.0f;
	float display_temp = 0.0f;
	float filtered_temp = 0.0f;

	//Variable to keep track of alarm status
	int is_alarm_on = 0;
 
	loop_count_gbl = 0;
	
  /* MCU Configuration----------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	
  /* Configure the system clock */
  SystemClock_Config();
	
	//Enable the GPIO clocks
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	//Enable the ADC clock
	__HAL_RCC_ADC1_CLK_ENABLE();
	
	//Configure the GPIO struct for the 4 LEDs on the discovery board
	GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; 
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; 
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST; 

	//Initialize GPIO for the LEDs
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	//Set ADC handler to ADC1
	ADC1_Handle.Instance = ADC1 ;

	//Configure ADC_InitTypeDef parameters
	ADC_InitStruct.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC_InitStruct.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4; 
	ADC_InitStruct.Resolution = ADC_RESOLUTION_12B; 
	ADC_InitStruct.ContinuousConvMode = ENABLE; 
	ADC_InitStruct.DiscontinuousConvMode = DISABLE; 
	ADC_InitStruct.NbrOfConversion = 1; 
	ADC_InitStruct.ScanConvMode = DISABLE; 
	ADC_InitStruct.DMAContinuousRequests = DISABLE; 
	//ADC_InitStruct.ExternalTrigConv = ADC_SOFTWARE_START;
	//ADC_InitStruct.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	
	//set ADC_HandleTypeDef parameters
	ADC1_Handle.Init = ADC_InitStruct; 

	//Initialize the ADC1
	if (HAL_ADC_Init(&ADC1_Handle) != HAL_OK){
		Error_Handler(ADC_INIT_FAIL);
	}
	
	//Configure the channel for the ADC 
	set_adc_channel(); 
	
	//Start polling mode
	if (HAL_ADC_Start(&ADC1_Handle) != HAL_OK) {
		Error_Handler(ADC_INIT_FAIL);
	}
	
	//Configure GPIOC for the 4 select lines
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pull = GPIO_NOPULL; 
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//Configure GPIOA for the segments
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pull = GPIO_NOPULL; 
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//Configure GPIOE for the LCD display
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2  | GPIO_PIN_7 | GPIO_PIN_8  | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL; 
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	//Initialize the LCD display
	LCD_init();
	
	//Initialize the Kalman Filter struct
	Kalmanfilter_init();
	
	printf("This is where the temp sensing starts\n");
	while (1){
		
		//If SysTick_Handler is called, wait as there is an interrupt

		if(HAL_ADC_PollForConversion(&ADC1_Handle, 1000000) != HAL_OK) {
				Error_Handler(ADC_INIT_FAIL);
		}
		//Check to see if the conversion to digital value is operational or 
		/*if (adc_started == 0)
		{
			if (HAL_ADC_Start(&ADC1_Handle) != HAL_OK) {
				Error_Handler(ADC_INIT_FAIL);}
			else adc_started = 1;
		}
		
		if (adc_started == 1) 
		{
			
			if(HAL_ADC_PollForConversion(&ADC1_Handle, 1000000) != HAL_OK) {
				Error_Handler(ADC_INIT_FAIL);
			}
			else
			{
				adc_valid = 1;		
			}
		}
		*/
		//printf("The tick count is %d\n", loop_count_gbl);

		//Resets counter to 0 when needed in keeping with the LED rotation tracking needs
		if(loop_count_gbl >= (ALARM_PERIOD*4)) {
			loop_count_gbl = 0;
		}
		
		if (loop_count_gbl % 10 == 0) {
				temperature = get_data_from_sensor();
		}
		
		//Measures temperature from sensor every 10ms -> 100Hz frequency
		/*if (adc_valid == 1)
		{
			if (loop_count_gbl % 10 == 0) {
				temperature = get_data_from_sensor();
			}
			adc_valid = 0;
			HAL_ADC_Stop(&ADC1_Handle);
			adc_started = 0;
		}*/
		
		//Capture temperature to display every 500ms
		if(loop_count_gbl % 250 == 0) {
			
			Kalmanfilter_C(temperature, &current_kstate);
			filtered_temp = current_kstate.x;
			
			printf("Temp: %f\t FilteredTemp: %f\n", temperature, filtered_temp);
			display_temp = filtered_temp;
			//LCD_display(25.2);
		}
		
		
		//Display the value caught at every 500ms interval to stabilize the 7seg display
		update_segment_display(display_temp);


		//Launches overheating alarm if the temperature is greater than the upper threshold
		if(filtered_temp > OVERHEAT_TEMP) {
			launch_overheat_alarm(loop_count_gbl);
			
			//Toggles the alarm status
			if(!is_alarm_on) {
				is_alarm_on = 1;
			}
			
		//Turn off alarm if temp is back to normal
		} else {
			
			//Turns off the alarm and toggles the alarm status boolean;
			if(is_alarm_on) {
				launch_overheat_alarm(ALARM_RESET);
				is_alarm_on = 0;
			}
		}
	}
}

/** System Clock Configuration */
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
	
	/*Configures SysTick to provide 1ms interval interrupts.*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/* This function sets the source clock for the internal SysTick Timer to be the maximum,
	   in our case, HCLK is now 168MHz*/
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
   * @brief A function used to setup the ADC to sample Channel 16
   * @retval None
   */
void set_adc_channel (void) {
	
	//sets ADC_ChannelConfTypeDef parameters
	ADC_ChannelStruct.Channel = ADC_CHANNEL_16;
	ADC_ChannelStruct.SamplingTime = ADC_SAMPLETIME_480CYCLES; 
	ADC_ChannelStruct.Rank = 1;
	ADC_ChannelStruct.Offset = 0;
	
	if (HAL_ADC_ConfigChannel(&ADC1_Handle, &ADC_ChannelStruct) != HAL_OK )
	{
		Error_Handler(ADC_CH_CONFIG_FAIL);
	}		
}

/**
   * @brief A function used to dictate the rotation of the LED alarm when the processor overheats
	 * @param tick_cnt: global loop count to help determine which LED to turn on when
	 * @retval None
   */
void launch_overheat_alarm (int tick_cnt) {
	
	//If true, reset the LEDs
	if(tick_cnt == ALARM_RESET) {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
	
	//Otherwise turn on LEDs in a rotating alarm
	} else {
		
		//if the tick count is between 0 and 500, turn LED 0(Blue) on
		if(tick_cnt >= 0 && tick_cnt < ALARM_PERIOD) {
			
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
		}
		
		//if the tick count is between 500 and 1000, turn LED 1(Green) on
		if(tick_cnt >= ALARM_PERIOD && tick_cnt < (ALARM_PERIOD*2)) {
			
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		}
		
		//if the tick count is between 1000 and 1500, turn LED 2(Orange) on
		if(tick_cnt >= (ALARM_PERIOD*2) && tick_cnt < (ALARM_PERIOD*3)) {
			
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		}
		
		//if the tick count is between 1500 and 2000, turn LED 3(Red) on
		if(tick_cnt >= (ALARM_PERIOD*3) && tick_cnt < (ALARM_PERIOD*4)) {
			
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
		}
	}
}

/**
   * @brief A function used to filter the noisy converted temperature readings from ADC
	 * @retval temperature: a float that contains the filtered temperature value
   */
/*void filter_temperature (float temperature) {
	//float filtered_temp;
	
	//call function Kalmanfilter_C
	Kalmanfilter_C(temperature, &current_kstate);
	
	//return filtered_temp;
}*/

/**
   * @brief A function used to initialize the kalman filter struct
	 * @param
	 * @retval filtered_temp: a float that contains the filtered temperature value
   */
void Kalmanfilter_init(void) {
	current_kstate.q = 0.02f; //process noise covariance
	current_kstate.r = 0.5f; //measurement noise covariance
	current_kstate.x = 28.0f; //estimated value
	current_kstate.p = 2.0f; //estimation error covariance
	current_kstate.k = 0.00f; //kalman gain
}

/**
   * @brief A function used to filter the noisy converted temperature readings from ADC
	 * @param measured_temp: this is the converted temperature reading input from the sensor
   * @param kstate: pointer to the kalman filter configuration struct
	 * @retval 
   */
int Kalmanfilter_C(float measured_temp, kalman_state* kstate) {	
	kstate->p = kstate->p + kstate->q;
	kstate->k = kstate->p / (kstate->p + kstate->r);
	kstate->x = kstate->x + kstate->k * (measured_temp-kstate->x);

	//The following conditional checks if kstate->x is not a number
	if (kstate->x != kstate->x) {
		printf("An error has occured -NaN value was found"); 
		return 1;
	}
	
	kstate->p = (1-kstate->k) * kstate->p;
	return 0; 
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
