/*******************************************************************************
  * @file    Thread_temp.c
  * @author  Taha Saifuddin, Richard Cheung
	* @version V1.0.0
  * @date    18th March, 2016
  * @brief   This file implements the display temp selection thread, and a  
  *					 function which creates and starts the thread	
  ******************************************************************************
  */

/* Includes ----------------------------------------------------------------- */
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "main.h"

/* Variables -----------------------------------------------------------------*/
int display_counter; 
int temp_flag =1;

/* Functions -----------------------------------------------------------------*/
void Thread_temp (void const *argument);                 // thread function

osThreadId tid_Thread_temp;                              // thread id
osThreadDef(Thread_temp, osPriorityNormal, 1, 0);				 // set thread priority


/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_temp (void) {

  tid_Thread_temp = osThreadCreate(osThread(Thread_temp), NULL); // Start calculation_Thread
  if (!tid_Thread_temp) return(-1); 
  return(0);
}

/*----------------------------------------------------------------------------
*      Thread  'Thread_temp': provide values for temperatures
 *---------------------------------------------------------------------------*/
void Thread_temp (void const *argument) {
	display_temp = 0.0f;
	display_counter = 0;
	
	while(1){
		osSignalWait(temp_flag, osWaitForever);
		osMutexWait(temp_mutex, osWaitForever);
		
		//Update the display value of the temp every 250ms
		if (display_mode == 0) {
			if (display_counter % 250 == 0) {
				//check_temperature_status(display_temp);
				display = display_temp;	
				display_counter = 0;
			}
		}		
		osMutexRelease(temp_mutex);
		osSignalClear(tid_Thread_temp, temp_flag);
	}
}

/**
   * @brief Sets the TIM3 interrupt flag to 1
   * @param *htim
   * @retval None
   */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//Increments TIM3 counter
	TIM3_counter ++;
	
	//Increments display_counter when in temp display mode
	if(display_mode == 0) display_counter++; 
	
	osSignalSet(tid_Thread_temp, temp_flag);
}
