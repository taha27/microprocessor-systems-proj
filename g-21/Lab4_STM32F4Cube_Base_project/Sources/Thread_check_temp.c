/*******************************************************************************
  * @file    Thread_check_temp.c
  * @author  Taha Saifuddin, Richard Cheung
	* @version V1.0.0
  * @date    18th March, 2016
  * @brief   This file implements the overheat temperature check thread, and a  
  *					 function which creates and starts the thread		
  ******************************************************************************
  */

/* Includes ----------------------------------------------------------------- */
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "main.h"

/* Variables -----------------------------------------------------------------*/
GPIO_InitTypeDef 				LED_configuration;

/* Functions -----------------------------------------------------------------*/
void Thread_check_temp (void const *argument);                 // thread function

osThreadId tid_Thread_check_temp;                              // thread id
osThreadDef(Thread_check_temp, osPriorityNormal, 1, 0);				 // set thread priority

/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_check_temp (void) {

  tid_Thread_check_temp = osThreadCreate(osThread(Thread_check_temp ), NULL); // Start Temp Check Thread
  if (!tid_Thread_check_temp) return(-1); 
  return(0);
}

 /*----------------------------------------------------------------------------
*      Implementation of the temperature checking thread
 *---------------------------------------------------------------------------*/
void Thread_check_temp (void const *argument) {
	
	float temperature = 0.0f;
	
	while(1){
		
		//Using a delay of 10ms so that the frequency of temperature sampling is 100Hz
		osDelay(10);
		
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
				temperature = get_data_from_sensor();
			}
			
			//Clear the EOC flag
			__HAL_ADC_CLEAR_FLAG(&ADC1_Handle,ADC_FLAG_EOC);
		}
		
		//Pass the temp value to the temperature checking function
		display_temp = temperature;
		check_temperature_status(display_temp);			
	}
}
