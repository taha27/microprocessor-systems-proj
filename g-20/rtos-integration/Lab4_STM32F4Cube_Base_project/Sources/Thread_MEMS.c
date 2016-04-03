/**
  ******************************************************************************
  * File Name          : Thread_MEMS.c
  * Description        : Worker thread source code for sampling accelerometer data
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 18th, 2016
  ******************************************************************************
  */
	
/* Includes */
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "LIS3DSH.h"
#include "math.h"
#include "7seg_display.h"
#include "kalman_filter.h"

/* Thread function prototypes */
void Thread_MEMS(void const *argument);
osThreadId tid_Thread_MEMS;
osThreadDef(Thread_MEMS, osPriorityNormal, 1, 0);

/* Global declarations */
TIM_HandleTypeDef TIM_HandleStruct;
float pitch, roll;
kalman_state kstate_x, kstate_y, kstate_z;

/* Extern declarations */
extern int interrupt;
extern osMutexId mems_mutex_id;

/**
	 * @brief Spawns the worker thread and checks for thread creation success
	 * @param void
   * @retval integer value showing status of the thread creation
   */
int start_Thread_MEMS(void){

  tid_Thread_MEMS = osThreadCreate(osThread(Thread_MEMS), NULL);
  if (!tid_Thread_MEMS) return(-1); 
  return(0);
}

/**
	 * @brief Worker thread main superloop that defines how the thread will always display values 
	 * depending on the relevant mode of operation
	 * @param void*
   * @retval void
   */
void Thread_MEMS(void const *argument){
	
	float read_acc[] = {0, 0, 0};
	double output_x, output_y, output_z;
	Reset_MEMS(&kstate_x);
	Reset_MEMS(&kstate_y);
	Reset_MEMS(&kstate_z);
	// double den_pitch, den_roll;
	
	while(1){
		osDelay(200);
		// printf("mems: works\n\r");
		// printf("mems: interrupt = %d\n\r", interrupt);
		if(interrupt != 0){
			interrupt = 0;
			// printf("mems: now interrupt = %d\n\r", interrupt);
			LIS3DSH_ReadACC(read_acc);	
			// printf("%f | %f | %f\n", read_acc[0], read_acc[1], read_acc[2]);
			
			if(!Kalmanfilter_C(read_acc[0], &output_x, &kstate_x) && !Kalmanfilter_C(read_acc[1], &output_y, &kstate_y) 
				&& !Kalmanfilter_C(read_acc[2], &output_z, &kstate_z)){
					
				// printf("x = %f out = %f\n", read_acc[0], output_x);
				// printf("y = %f out = %f\n", read_acc[1], output_y);
				// printf("z = %f out = %f\n", read_acc[2], output_z);
					
				output_x = output_x + 0.10493;
				output_y = output_y + 0.143217;
				output_z = output_z + 0.665265;
			
				osMutexWait(mems_mutex_id, osWaitForever);
				// roll  = (atan2(-fYg, fZg)*180.0)/M_PI;
				roll  = (atan2(-output_y, output_z) * 180.0) / 3.1416;
				osMutexRelease(mems_mutex_id);
				// printf("mems: roll= %f\n", roll);
				
				osMutexWait(mems_mutex_id, osWaitForever);	
				// pitch = (atan2(fXg, sqrt(fYg*fYg + fZg*fZg))*180.0)/M_PI;			
				pitch = (atan2(output_x, sqrt(output_y * output_y + output_z * output_z))*180.0) / 3.1416;
				osMutexRelease(mems_mutex_id);
				// printf("mems: roll= %f\n", pitch);
			}
			else printf("mems: Kalman filter returned error!\n");
		}
	}
}


	
