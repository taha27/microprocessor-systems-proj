/**
  ******************************************************************************
  * File Name          : accelerometer.h
  * Description        : Accelerometer functions that can be used by other classes
	* Author						 : Taha Saifuddin, Richard Cheung
	* Version            : 1.0.0
	* Date							 : March 9th, 2016
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Structs -------------------------------------------------------------------*/
typedef struct stateInfo{
	float q, r, x, p, k;
}kalman_state;


/* Exported functions ------------------------------------------------------- */
extern void configure_init_accelerometer(void);
extern void configure_interrupt_line(void);
extern void get_calibrated_acceleration(void);
extern void Kalmanfilter_init(void);
extern void compare_user_actual_angle(void);
extern void config_animation_LEDs(void);
/* Exported Variables ------------------------------------------------------- */
extern float pitch, roll;

#endif /** ACCELEROMETER_H */
