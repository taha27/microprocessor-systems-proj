/**
  ******************************************************************************
  * @file      : temperature_sensor.c
  * @brief     : This source file enables reading temp sensor voltage and its 
							   conversion to celcius, and checking if it's in a safe range
	* @author		 : Taha Saifuddin, Richard Cheung
	* @version   : 2.0.0
	* @date			 : March 18th, 2016
  ******************************************************************************
  */

/* Includes ----------------------------------------------------------------- */
#include "temperature_sensor.h"

/* Defines ------------------------------------------------------------------ */
#define OVERHEAT_TEMP		(float)35.0

/* Private variables ---------------------------------------------------------*/
int is_alarm_on;
ADC_InitTypeDef ADC_InitStruct;
ADC_ChannelConfTypeDef ADC_ChannelStruct;

/* Global variables ----------------------------------------------------------*/
ADC_HandleTypeDef ADC1_Handle;

/* Private functions ---------------------------------------------------------*/
void set_adc_channel (void);

/**
   * @brief A function used to configure and initialize the temperature sensor
	 * @param none
	 * @retval none
   */
void configure_init_temp_sensor(void)
{
	//Set ADC handler to ADC1
	ADC1_Handle.Instance = ADC1 ;

	//Configure ADC_InitTypeDef parameters
	ADC_InitStruct.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC_InitStruct.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4; 
	ADC_InitStruct.Resolution = ADC_RESOLUTION_12B; 
	ADC_InitStruct.ContinuousConvMode = DISABLE; 
	ADC_InitStruct.DiscontinuousConvMode = DISABLE; 
	ADC_InitStruct.NbrOfConversion = 1; 
	ADC_InitStruct.ScanConvMode = DISABLE; 
	ADC_InitStruct.DMAContinuousRequests = DISABLE; 
	ADC_InitStruct.ExternalTrigConv = ADC_SOFTWARE_START;
	ADC_InitStruct.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADC_InitStruct.EOCSelection = ENABLE;
	
	//set ADC_HandleTypeDef parameters
	ADC1_Handle.Init = ADC_InitStruct; 

	//Initialize the ADC1
	if (HAL_ADC_Init(&ADC1_Handle) != HAL_OK){
		Error_Handler(ADC_INIT_FAIL);
	}
	
	//Configure the channel for the ADC 
	set_adc_channel(); 
	
	//Start polling mode - commented out as we are going to do it in 
	//each iteration of the while loop of the check_temp thread
	/*if (HAL_ADC_Start(&ADC1_Handle) != HAL_OK) {
		Error_Handler(ADC_START_FAIL);
	}*/
}

/**
   * @brief A function used to setup the ADC to sample Channel 16
   * @retval None
   */
void set_adc_channel (void) {
	
	//sets ADC_ChannelConfTypeDef parameters
	ADC_ChannelStruct.Channel = ADC_CHANNEL_16;
	ADC_ChannelStruct.SamplingTime = ADC_SAMPLETIME_480CYCLES; 
	ADC_ChannelStruct.Rank = 1;
	ADC_ChannelStruct.Offset = 0;
	
	if (HAL_ADC_ConfigChannel(&ADC1_Handle, &ADC_ChannelStruct) != HAL_OK )
	{
		Error_Handler(ADC_CH_CONFIG_FAIL);
	}		
}

/**
   * @brief A function used to convert the sensor data to temperature in celcius
	 * @param voltage: the scaled voltage value at the current temperature
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
	
	float voltage = 0.0f;
	float temperature = 0.0f;
	
	//Get the raw data from the internal temperature sensor
	voltage = HAL_ADC_GetValue(&ADC1_Handle);
	
	//printf("The voltage from sensor is %f\n", voltage);
	
	//Filter the sensor data
	Kalmanfilter_C(voltage, &temp_kstate);
	voltage = temp_kstate.x;
	
	//Apply the proper scaling to convert to voltage from base 12 ADC equivalent
	voltage = (3 * voltage) / 4096;
	
	//Convert the voltage to celcius
	temperature = convert_voltage_to_celcius(voltage);
	
	return temperature;
}

/**
   * @brief A function used to check if the processor is overheating, and launch the alarm if so
	 * @param temperature
	 * @retval none
   */
void check_temperature_status(float temperature)
{
	//Launches overheating alarm if the temperature is greater than the upper threshold
	if(temperature > OVERHEAT_TEMP) {
		flash_segment_display_alarm();
	}
	else {
		is_alarm_on = 0;
		count_for_alarm = 0;
	}
}

