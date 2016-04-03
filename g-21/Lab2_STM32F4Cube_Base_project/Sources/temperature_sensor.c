/**
  ******************************************************************************
  * File Name          : temperature_sensor.c
  * Description        : This source file enables reading temp sensor voltage
												 and its conversion to celcius
	* Author						 : Richard Cheung, Taha Saifuddin
	* Version            : 1.0.0
	* Date							 : February 19th, 2016
  ******************************************************************************
  */

#include "temperature_sensor.h"

/**
   * @brief A function used to convert the sensor data to temperature in celcius
	 * @retval temperature_celcius: float containing the temperature value in celcius
   */
float convert_voltage_to_celcius (float voltage) {
	float temperature_celcius;
	
/** Temperature(°C) = ((V_SENSE - V_25) / Avg_Slope) + 25
* where V_SENSE is voltage read from the internal temp sensor
*				V_25 is the reference voltage at 25°C -> 0.76V
*				Avg_Slope is the average slope of temperature vs V_SENSE curve -> 2.5mV/°C
**/
	temperature_celcius = (((voltage - 0.76f) / 0.0025f) + 25.0f);
	
	return temperature_celcius;
}

/**
   * @brief A function used to get the sensor data from ADC
	 * @retval temperature: float containing the temperature value
   */
float get_data_from_sensor (void) {
	
	float voltage;
	float temperature;
	
	//Get the raw data from the internal temperature sensor
	voltage = HAL_ADC_GetValue(&ADC1_Handle);
	
	//printf("The voltage from sensor is %f\n", voltage);
	
	//Apply the proper scaling to convert to voltage from base 12 ADC equivalent
	voltage = (3 * voltage) / 4096;
	
	//Convert the voltage to celcius
	temperature = convert_voltage_to_celcius(voltage);
	
	//Filter the sensor data
	//temperature = filter_sensor_data(temperature);
	
	return temperature;
}

