/**
  ******************************************************************************
  * File Name          : get_readings.h
  * Description        : Header file for "get_readings.c"
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 11th, 2016
  ******************************************************************************
  */

/* Includes */
#include "kalman_filter.h"
#include "math.h"
#include "lis3dsh.h"
#include "7seg_display.h"
#define ARM_MATH_CM4
#include "arm_math.h"

/* Extern declarations */
extern volatile int interrupt;
extern float read_acc[];
extern float output_x[];
extern float output_y[];
extern float output_z[];
extern kalman_state kstate_x;
extern kalman_state kstate_y;
extern kalman_state kstate_z;
extern float den_pitch, den_roll;
extern float pitch, roll;
extern float diff_in, diff_out;

/* Function prototypes */
void Get_Sensor_Data(float alarm);
