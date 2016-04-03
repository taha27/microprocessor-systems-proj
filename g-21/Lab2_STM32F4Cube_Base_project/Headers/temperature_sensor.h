/**
  ******************************************************************************
  * File Name          : temperature_sensor.h
  * Description        : Temperature sensor functions that can be used by other classes
	* Author						 : Taha Saifuddin, Richard Cheung
	* Version            : 1.0.0
	* Date							 : February 19th, 2016
  ******************************************************************************
  */
	
#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include "main.h"
#include "stm32f4xx_hal.h"

extern float get_data_from_sensor(void);
extern float convert_voltage_to_celcius (float voltage);

#endif /** TEMPERATURE_SENSOR_H */
