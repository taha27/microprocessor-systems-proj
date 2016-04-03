/*******************************************************************************
  * @file    main.c
  * @author  Aditya Saha & Habib Ahmed (original template by Ashraf Suyyagh)
	* @version V1.2.0
  * @date    17-January-2016
  * @brief   Main thread that spawns and configures other worker threads 
  ******************************************************************************
  */

/* Includes */
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "RTE_Components.h"
#include <stdio.h>
#include "system_config.h"

/* Function prototypes */
int fputc(int ch, FILE *f);
extern int start_Thread_ADC(void);
extern int start_Thread_DISP(void);
extern int start_Thread_MEMS(void);

/* Mutex declarations */
osMutexDef(temp_mutex);
osMutexId(temp_mutex_id);
osMutexDef(mems_mutex);
osMutexId(mems_mutex_id);

/**
	These lines are mandatory to make CMSIS-RTOS RTX work with te new Cube HAL
*/
#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;

uint32_t HAL_GetTick(void){ 
	
  return os_time; 
}
#endif

/**
	 * @brief Main thread that configures all peripherals and timers, initializes OS
	 * configurations and spawns worker threads required for operation
	 * @param void
   * @retval integer value showing correct operation of main thread
   */
int main(void){

  osKernelInitialize();

  HAL_Init();
  SystemClock_Config();
	ADC_Config();
	GPIO_Config();
	MEMS_Config();
	MEMS_Config_IT();
	TIM3_Config();
	
	temp_mutex_id = osMutexCreate(osMutex(temp_mutex));
	mems_mutex_id = osMutexCreate(osMutex(mems_mutex));
	
	start_Thread_ADC();	
	start_Thread_DISP();	
	start_Thread_MEMS();
	
	osKernelStart();
}

/**
	 * @brief Retarget fputc for printf application
	 * @param ch- integer value of desired character to be displayed
	 * @param f- file pointer, specifying stdout in this case
   * @retval integer value of extracted character
   */
int fputc(int ch, FILE *f){
	return ITM_SendChar(ch);
}
	
