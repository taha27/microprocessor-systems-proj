/**
  ******************************************************************************
  * @file       : accelerometer.h
  * @brief      : Accelerometer functions that can be used by other classes
	* @author			: Taha Saifuddin, Richard Cheung
	* @version    : 2.0.0
	* @date				: March 18th, 2016
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Structs -------------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
extern void configure_init_accelerometer(void);
extern void configure_interrupt_line(void);
extern void get_calibrated_acceleration(void);
extern void Kalmanfilter_init(void);
//extern void compare_user_actual_angle(void);
//extern void config_animation_LEDs(void);

/* Exported Variables ------------------------------------------------------- */
extern float pitch, roll;

#endif /** ACCELEROMETER_H */
