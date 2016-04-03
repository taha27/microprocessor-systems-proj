/**
  ******************************************************************************
  * File Name          : Thread_DISP.c
  * Description        : Worker thread source code for displaying on 7-segment
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 18th, 2016
  ******************************************************************************
  */

/* Includes */
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "7seg_display.h"
#include "keypad.h"

/* Thread function prototypes */
void Thread_DISP(void const *argument);
osThreadId tid_Thread_DISP;
osThreadDef(Thread_DISP, osPriorityNormal, 1, 0);

/* Extern declarations */
extern osMutexId temp_mutex_id;
extern osMutexId mems_mutex_id;
extern int interrupt4, interrupt5;
extern int interrupt2, interrupt3;
extern double output;
extern float pitch, roll;
extern int interrupt7;

/* Global declarations */
int displaying[] = {0, 0, 0, 0, 0};
int parsed[] = {0, 0, 0, 0, 0};
int mode = 1; /* mode{0} = ADC, mode{1} = MEMS */
int mode1 = 1; /* mode1{0} = roll, mode1{1} = pitch */

/* Function prototypes */
void Show(void);
void Show_Negative(void);

/**
	 * @brief Spawns the worker thread and checks for thread creation success
	 * @param void
   * @retval integer value showing status of the thread creation
   */
int start_Thread_DISP(void){

  tid_Thread_DISP = osThreadCreate(osThread(Thread_DISP), NULL);
  if (!tid_Thread_DISP) return(-1); 
  return(0);
}

/**
	 * @brief Worker thread main superloop that defines how the thread will always display values 
	 * depending on the relevant mode of operation
	 * @param void*
   * @retval void
   */
void Thread_DISP(void const *argument){
	
	double temp2;
	int disp[] = {0, 0, 0, 0}; 
	int* temp3;
	float roll_temp, pitch_temp;
	int final_col = 0; 
	int sample_col;
	interrupt7 = 0;
	
	while(1){
		
		sample_col = Get_Column();
		if(sample_col != NULL){ 
			if(interrupt7 > 50){ 
				// printf("column= %d\n", sample_col);
				final_col = sample_col;
				interrupt7 = 0;
			}					
		}
		
		if(final_col != 0){ 
			
			// printf("col= %d\n", final_col);
			if(final_col == 4){
				mode = 0;
			}
			else if(final_col == 5){
				mode = 1;
				mode1 = 0;
			}
			else{
				mode = 1;
				mode1 = 1;
			}
		}
		
		if(mode == 0){
		
			// osDelay(2);
			osMutexWait(temp_mutex_id, osWaitForever);
			temp2 = output;
			osMutexRelease(temp_mutex_id);
			// printf("disp: temp2= %f\n", temp2);		
			Parse_Temp(temp2, disp);
			// printf("%d %d %d %d\n", disp[3], disp[2], disp[1], disp[0]);
			
			if(interrupt5 == 1){
				temp3 = disp;
			}
			
			if(interrupt4 <= 2){
				if(temp3[3] == 4 && temp3[2] > 0 && interrupt5 > 600) Blink();
				else Display(temp3[3], 4);
			}
			
			else if(interrupt4 <= 4){
				if(temp3[3] == 4 && temp3[2] > 0 && interrupt5 > 600) Blink();
				else Display(temp3[2], 3);
			}
			
			else if(interrupt4 <= 6){
				if(temp3[3] == 4 && temp3[2] > 0 && interrupt5 > 600) Blink();
				else Display(temp3[0], 1);
			}
			
			else if(interrupt4 <= 8){
				if(temp3[3] == 4 && temp3[2] > 0 && interrupt5 > 600) Blink();
				else Display(11, 0);
			}
			
			else{
				if(temp3[3] == 4 && temp3[2] > 0 && interrupt5 > 600) Blink();
				else Display(temp3[1], 2);
			}

			/* Reset loop counter after 90 cycles */
			if(interrupt5 > 1200){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
				interrupt5 = 0;					
			}
			
			/* Reset loop counter after 4 cycles */
			if(interrupt4 > 10){
				interrupt4 = 0;
			}
		}
		
		else{
		
			if(mode1 == 0){
				
				// osDelay(2);
				osMutexWait(mems_mutex_id, osWaitForever);
				roll_temp = roll;
				osMutexRelease(mems_mutex_id);
				// printf("mems: roll= %f\n", roll_temp);
				// printf("mems: temp= %f\n", output);
						
				if(roll_temp < 100){
					Parse_Mems(parsed, roll_temp);								
					// printf("disp2: %d %d %d %d %d\n", parsed[3], parsed[2], parsed[1], parsed[0], parsed[4]);
				}
				
				else{
					parsed[0] = (int) roll_temp % 10;
					parsed[2] = ((int) roll_temp / 10) % 10;
					parsed[3] = ((int) roll_temp / 100) % 10;
					parsed[1] = 0;
					parsed[4] = 0;				
					// printf("disp2: %d %d %d %d %d\n", parsed[3], parsed[2], parsed[1], parsed[0], parsed[4]);
				}
			}
			
			else{
			
				// osDelay(2);
				osMutexWait(mems_mutex_id, osWaitForever);
				pitch_temp = pitch;
				osMutexRelease(mems_mutex_id);
				// printf("mems: roll= %f\n", pitch_temp);
				// printf("mems: temp= %f\n", output);
				
				if(pitch_temp < 100){
					Parse_Mems(parsed, pitch_temp);								
					// printf("disp2: %d %d %d %d %d\n", parsed[3], parsed[2], parsed[1], parsed[0], parsed[4]);
				}
				
				else{
					parsed[0] = (int) pitch_temp % 10;
					parsed[2] = ((int) pitch_temp / 10) % 10;
					parsed[3] = ((int) pitch_temp / 100) % 10;
					parsed[1] = 0;
					parsed[4] = 0;				
					// printf("disp2: %d %d %d %d %d\n", parsed[3], parsed[2], parsed[1], parsed[0], parsed[4]);
				}
			}
			
			if(parsed[4] < 0 || parsed[3] < 0 || parsed[2] < 0 || parsed[1] < 0 || parsed[0] < 0) Show_Negative();
			else Show();
		}
	}
}
	
/**
	 * @brief Displays a unique combination of segments to signify negative valued inputs
	 * @param void
   * @retval void
   */
void Show_Negative(void){

	if(interrupt2 < 3){
		if(output > 40 && interrupt2 > 1);
		else Display(12, 4);
	}
	
	else if(interrupt2 < 6){
		if(output > 40 && interrupt2 > 4);
		else Display(12, 3);
	}
	
	else{
		if(output > 40 && interrupt2 > 7);
		else Display(12, 1);
	}
	
	if(interrupt2 > 10){
		interrupt2 = 0;
	}
}

/**
	 * @brief Display the parsed values on the 7-segment display
	 * @param void
   * @retval void
   */
void Show(void){
		
	// printf("%d %d %d %d\n", displaying[3], displaying[2], displaying[1], displaying[0]);
	
	if(interrupt3 == 1){
		displaying[3] = parsed[3];
		displaying[2] = parsed[2];
		displaying[1] = parsed[1];
		displaying[0] = parsed[0];
		displaying[4] = parsed[4];
	}
	
	if(interrupt2 < 2){
		if(displaying[3] != 0){
			if(output > 40 && interrupt3 > 600) Blink();
			else Display(displaying[3], 4);
		}
		else{
			if(output > 40 && interrupt3 > 600) Blink();
			else Display(displaying[2], 4);
		}
	}
	
	else if(interrupt2 < 4){
		if(displaying[3] != 0){
			if(output > 40 && interrupt3 > 600) Blink();
			else Display(displaying[2], 3);
		}
		else{
			if(output > 40 && interrupt3 > 600) Blink();
			else Display(displaying[1], 4);
		}
	}
	
	else if(interrupt2 < 6){
		if(displaying[3] != 0){
			if(displaying[1] != 0){
				if(output > 40 && interrupt3 > 600) Blink();
				else Display(displaying[1], 3);
			}
		}
		else{
			if(output > 40 && interrupt3 > 600) Blink();
			else Display(displaying[0], 3);
		}
	}
	
	else if (interrupt2 < 8){	
		if(displaying[3] != 0){		
			if(output > 40 && interrupt3 > 600) Blink();
			else Display(displaying[0], 1);					
		}
		else{
			if(output > 40 && interrupt3 > 600) Blink();
			else Display(displaying[4], 1);
		}
	}
	
	else{
		if(output > 40 && interrupt3 > 600) Blink();
		else Display(11, 0);
	}

	if(interrupt3 > 1200){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
		interrupt3 = 0;					
	}
	
	if(interrupt2 > 10){
		interrupt2 = 0;
	}	
}
	

