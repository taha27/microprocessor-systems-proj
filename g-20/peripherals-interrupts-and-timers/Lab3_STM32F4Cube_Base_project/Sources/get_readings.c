/**
  ******************************************************************************
  * File Name          : get_readings.c 
  * Description        : Cardinal function for reception, processing and display of sensor data
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 11th, 2016
  ******************************************************************************
  */

/* Includes */
#include "get_readings.h"

/**
	 * @brief Receives sensor data upon interrupt, processes values via Kalman filter, 
							calculates the assosciated tilt angles and displays relevant values
	 * @param void
   * @retval void
   */
void Get_Sensor_Data(float alarm){
	
	// printf("Input angle = %f\n", alarm);
	
	// printf("main: counter = %d, interrupt = %d\n", counter, interrupt);
	if(interrupt != 0){
		interrupt = 0;
		// printf("main: counter = %d, interrupt = %d\n", counter, interrupt);
		
		LIS3DSH_ReadACC(read_acc);	
		// printf("%f | %f | %f\n", read_acc[0], read_acc[1], read_acc[2]);
		
		if(!Kalmanfilter_C(read_acc[0], output_x, &kstate_x) && !Kalmanfilter_C(read_acc[1], output_y, &kstate_y) 
				&& !Kalmanfilter_C(read_acc[2], output_z, &kstate_z)){				
				
			// printf("%f | %f | %f\n", output_x[0], output_y[0], output_z[0]);
			output_x[0] = output_x[0] + 0.10493;
			output_y[0] = output_y[0] + 0.143217;
			output_z[0] = output_z[0] + 0.665265;					
			
			den_pitch = sqrt(pow(output_y[0], 2) + pow(output_z[0], 2));
			den_roll 	= sqrt(pow(output_x[0], 2) + pow(output_z[0], 2));
			// pitch = atan(output_x[0] / den_pitch) * (180 / 3.1416);
			// roll = atan(output_y[0] / den_roll)  * (180 / 3.1416);
			// printf("%f | %f\n", pitch, roll);	
			
			roll  = (atan2(-output_y[0], output_z[0])*180.0)/3.1416;
			if(roll < 100){
				Parse(parsed, roll);								
				// printf("%d %d %d %d %d\n", parsed[3], parsed[2], parsed[1], parsed[0], parsed[4]);
			}
			
			else{
				parsed[0] = (int) roll % 10;
				parsed[2] = ((int) roll / 10) % 10;
				parsed[3] = ((int) roll / 100) % 10;
				parsed[1] = 0;
				parsed[4] = 0;				
				// printf("%d %d %d %d %d\n", parsed[3], parsed[2], parsed[1], parsed[0], parsed[4]);
			}
		}
	}		
	if(parsed[4] < 0 || parsed[3] < 0 || parsed[2] < 0 || parsed[1] < 0 || parsed[0] < 0) Show_Negative();
	else Show();		
	
	diff_in = roll - alarm;
	arm_abs_f32(&diff_in, &diff_out, 1);
	if(diff_out <= 4)Display_Alarm();
	else{
		if(roll < alarm)Move_Up();
		else Move_Down();
	}
	// Reset_Alarm();	
}
