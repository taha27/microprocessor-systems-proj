/*******************************************************************************
  * @file    main.c
  * @author  Taha Saifuddin, Richard Cheung
	* @version V1.2.0
  * @date    18th March 2016
  * @brief   This file demonstrates flasing one LED at an interval of one second
	*          RTX based using CMSIS-RTOS 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "SPI_Config.h"

/* Variables -----------------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStruct;
kalman_state x_kstate, y_kstate, z_kstate, temp_kstate;

int EXTI0_flag_value;
int TIM3_flag_value;
SPI_HandleTypeDef SpiHandle_2;
int temperature;
uint8_t pBuffer[5];

/**
	These lines are mandatory to make CMSIS-RTOS RTX work with te new Cube HAL
*/
#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;

uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif

/**
  * System Clock Configuration
  */
void SystemClock_Config(void) {
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

int main(void)
{	
	//osKernelInitialize();                     /* initialize CMSIS-RTOS          */
 /* MCU Configuration----------------------------------------------------------*/
	HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	
  /* Initialize all configured peripherals */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	 //Enable the ADC clock
	__HAL_RCC_ADC1_CLK_ENABLE();
	 
	//Initialize the Accelerometer and the external interrup line 0
	configure_init_accelerometer();
	configure_interrupt_line();
	
	//Configure and initialize the ADC
	configure_init_temp_sensor();
	
//	//Initialize the kalman filter structs
	Kalmanfilter_init();
	
	//SPI
	SPI_Init(); 

	//start_packet_thread();

 /* start thread execution         */
	//osKernelStart();   
	while(1) {
	if(EXTI0_flag_value == INTERRUPT_ACTIVE_FLAG) {
				
			//Get the accelerometer readings
			get_calibrated_acceleration();
			
			//Store pitch angle for display
			pBuffer[0] = roll;

			//Store roll angle for display
			pBuffer[1] = pitch;
		

		
			//Start the ADC
			if (HAL_ADC_Start(&ADC1_Handle) != HAL_OK) {
				Error_Handler(ADC_START_FAIL);
			} 
			else {
				
				//Check for successful conversion of the polled analog value to a digital reading
				if(HAL_ADC_PollForConversion(&ADC1_Handle, 1000000) != HAL_OK) {
					Error_Handler(ADC_POLL_FAIL);
				} 
				else {
					//Measure temperature from sensor 
					pBuffer[2] = get_data_from_sensor();
				}
				
				//Clear the EOC flag
				__HAL_ADC_CLEAR_FLAG(&ADC1_Handle,ADC_FLAG_EOC);
			}
			
			//Reset the flag
			EXTI0_flag_value = 0;		
		}
		HAL_SPI_Transmit(&SpiHandle_2, pBuffer, 5, 100);
	}
			
}


//------------------------------------------------------------------------------
/**
   * @brief A function used to filter the noisy acceleration readings
	 * @param 
   * @param kstate: pointer to the kalman filter configuration struct
	 * @retval 
   */
int Kalmanfilter_C(float measured_acceleration, kalman_state* kstate) {	
	kstate->p = kstate->p + kstate->q;
	kstate->k = kstate->p / (kstate->p + kstate->r);
	kstate->x = kstate->x + kstate->k * (measured_acceleration-kstate->x);

	//The following conditional checks if kstate->x is not a number
	if (kstate->x != kstate->x) {
		printf("An error has occured -NaN value was found"); 
		return 1;
	}
	
	kstate->p = (1-kstate->k) * kstate->p;
	return 0; 
}

/**
   * @brief A function used to initialize the kalman filter struct for all kalman state objects
	 * @param	none
	 * @retval none
   */
void Kalmanfilter_init(void) {
	
	//Kalman state struct to filter Ax values
	x_kstate.q = 0.10f; //process noise covariance
	x_kstate.r = 2.00f; //measurement noise covariance
	x_kstate.x = 0.0f; //estimated value
	x_kstate.p = 2.0f; //estimation error covariance
	x_kstate.k = 0.0f; //kalman gain
	
	//Kalman state struct to filter Ay values
	y_kstate.q = 0.10f; //process noise covariance
	y_kstate.r = 2.00f; //measurement noise covariance
	y_kstate.x = 0.0f; //estimated value
	y_kstate.p = 2.0f; //estimation error covariance
	y_kstate.k = 0.0f; //kalman gain
	
	//Kalman state struct to filter Az values
	z_kstate.q = 0.10f; //process noise covariance
	z_kstate.r = 2.00f; //measurement noise covariance
	z_kstate.x = 1000.0f; //estimated value
	z_kstate.p = 2.0f; //estimation error covariance
	z_kstate.k = 0.0f; //kalman gain
	
	//Kalman state struct to filter Temp values
	temp_kstate.q = 0.02f; //process noise covariance
	temp_kstate.r = 0.5f; //measurement noise covariance
	temp_kstate.x = 28.0f; //estimated value
	temp_kstate.p = 2.0f; //estimation error covariance
	temp_kstate.k = 0.00f; //kalman gain
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	
	//Set EXTI0 interrupt flag to true
	EXTI0_flag_value = 1;
}
