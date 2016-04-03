/**
  ******************************************************************************
  * @file      : temperature_sensor.h
  * @brief     : This source file enables reading temp sensor voltage and its 
							   conversion to celcius, and checking if it's in a safe range
	* @author		 : Taha Saifuddin, Richard Cheung
	* @version   : 2.0.0
	* @date			 : March 18th, 2016
  ******************************************************************************
  */

#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

/* Includes ----------------------------------------------------------------- */
#include "main.h"
#include "stm32f4xx_hal.h"

/* Exported types ----------------------------------------------------------- */
extern ADC_HandleTypeDef ADC1_Handle;
extern int is_alarm_on;

/* Exported functions ------------------------------------------------------- */
extern void configure_init_temp_sensor(void);
extern float get_data_from_sensor(void);
extern float convert_voltage_to_celcius(float voltage);
extern void check_temperature_status(float temperature);

#endif /** TEMPERATURE_SENSOR_H */
