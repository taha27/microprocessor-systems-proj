#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "7seg_display.h"

void Thread_DISP2(void const *argument);
osThreadId tid_Thread_DISP2;
osThreadDef(Thread_DISP2, osPriorityNormal, 1, 0);

extern float pitch, roll;
extern osMutexId mems_mutex_id;
extern int interrupt2, interrupt3;
extern double output;

void Show(void);
void Show_Negative(void);

int displaying[] = {0, 0, 0, 0, 0};
int parsed[] = {0, 0, 0, 0, 0};

int start_Thread_DISP2(void){

  tid_Thread_DISP2 = osThreadCreate(osThread(Thread_DISP2), NULL); // Start LED_Thread
  if (!tid_Thread_DISP2) return(-1); 
  return(0);
}

void Thread_DISP2(void const *argument){
	
	float roll_temp, pitch_temp;
	
	while(1){
		// osDelay(10);
		osMutexWait(mems_mutex_id, osWaitForever);
		roll_temp = roll;
		osMutexRelease(mems_mutex_id);
		// printf("mems: roll= %f\n", roll_temp);
		printf("mems: temp= %f\n", output);
				
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
		
		if(parsed[4] < 0 || parsed[3] < 0 || parsed[2] < 0 || parsed[1] < 0 || parsed[0] < 0) Show_Negative();
		else Show();
	}
}

void Show_Negative(void){

	if(interrupt2 < 3){
		if(output > 32 && interrupt2 > 1);
		else Display(12, 4);
	}
	
	else if(interrupt2 < 6){
		if(output > 32 && interrupt2 > 4);
		else Display(12, 3);
	}
	
	else{
		if(output > 32 && interrupt2 > 7);
		else Display(12, 1);
	}
	
	if(interrupt2 > 10){
		interrupt2 = 0;
	}
}

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
			if(output > 32 && interrupt3 > 600) Blink();
			else Display(displaying[3], 4);
		}
		else{
			if(output > 32 && interrupt3 > 600) Blink();
			else Display(displaying[2], 4);
		}
	}
	
	else if(interrupt2 < 4){
		if(displaying[3] != 0){
			if(output > 32 && interrupt3 > 600) Blink();
			else Display(displaying[2], 3);
		}
		else{
			if(output > 32 && interrupt3 > 600) Blink();
			else Display(displaying[1], 4);
		}
	}
	
	else if(interrupt2 < 6){
		if(displaying[3] != 0){
			if(displaying[1] != 0){
				if(output > 32 && interrupt3 > 600) Blink();
				else Display(displaying[1], 3);
			}
		}
		else{
			if(output > 32 && interrupt3 > 600) Blink();
			else Display(displaying[0], 3);
		}
	}
	
	else if (interrupt2 < 8){	
		if(displaying[3] != 0){		
			if(output > 32 && interrupt3 > 600) Blink();
			else Display(displaying[0], 1);					
		}
		else{
			if(output > 32 && interrupt3 > 600) Blink();
			else Display(displaying[4], 1);
		}
	}
	
	else{
		if(output > 32 && interrupt3 > 600) Blink();
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
	

