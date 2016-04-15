/**
  ******************************************************************************
  * @file     : accelerometer.c
  * @brief    : This class provides functionality for configuring and 
								controlling the accelerometer
	* @author		: Taha Saifuddin, Richard Cheung
	* @version	: 2.0.0
	* @date			: March 18th, 2016
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "accelerometer.h"
#include "math.h"

/* Defines -------------------------------------------------------------------*/
//#define THRESHOLD_TOLERANCE 5.0f

/* Variables -----------------------------------------------------------------*/
LIS3DSH_InitTypeDef LIS3DSH_InitStruct;
LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_IntConfigStruct;
GPIO_InitTypeDef GPIO_InitStructx;
float acceleration_reading[3];		
float acceleration_filtered[3];
float acceleration_normalized[3];
int i;
int pitch;
int roll;
int count_for_animation;

float calibration_param_matrix[4][3] = {
	{0.000975054748, -0.0000182180993, -0.0000000305652774},
	{-0.0000307680088, 0.00100024108, -0.00000438213129},
	{-0.00000617881222, -0.00000113744427, 0.000963388786},
	{0.00214820029, 0.00162097280, -0.0131357405}
};


/* Private function prototypes -----------------------------------------------*/
void print_filtered_acceleration(void);
//int Kalmanfilter_C(float measured_acceleration, kalman_state* kstate);
void calculate_angles(void);

/**
   * @brief A function used to configure and init the accelerometer
	 * @param none
	 * @retval none
   */
void configure_init_accelerometer(void)
{
	//Configure the accelerometer initialization struct
	LIS3DSH_InitStruct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_100;        /* Ppower down or /active mode with output data rate 3.125 / 6.25 / 12.5 / 25 / 50 / 100 / 400 / 800 / 1600 HZ */
  LIS3DSH_InitStruct.Axes_Enable = LIS3DSH_XYZ_ENABLE;                        /* Axes enable */
  LIS3DSH_InitStruct.Continous_Update = LIS3DSH_ContinousUpdate_Disabled;			/* Block or update Low/High registers of data until all data is read */
	LIS3DSH_InitStruct.AA_Filter_BW = LIS3DSH_AA_BW_50;													/* Choose anti-aliasing filter BW 800 / 400 / 200 / 50 Hz*/
  //closest to sampling rate
	LIS3DSH_InitStruct.Full_Scale = LIS3DSH_FULLSCALE_2;                        /* Full scale 2 / 4 / 6 / 8 / 16 g */
  LIS3DSH_InitStruct.Self_Test = LIS3DSH_SELFTEST_NORMAL;                     /* Self test */
	//Initialize the accelerometer
	LIS3DSH_Init(&LIS3DSH_InitStruct);
	
	//Configure the accelerometer interrupt struct
	LIS3DSH_IntConfigStruct.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;     /* Enable/Disable data ready interrupt */
  LIS3DSH_IntConfigStruct.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;         /* Interrupt Signal Active Low / Active High */
  LIS3DSH_IntConfigStruct.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;							/* Interrupt type as latched or pulsed */
	//
	//Setting the interrupt configuration of the accelerometer
	LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_IntConfigStruct);
	
	//Enable the interrupt line
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
		
}

/**
   * @brief A function used to configure and init the external interrupt line 0
						for use with the accelerometer
	 * @param none
	 * @retval none
   */
void configure_interrupt_line(void)
{
	//Configure GPIOE for the interrupt line
	GPIO_InitStructx.Pin = GPIO_PIN_0;
	GPIO_InitStructx.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructx.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructx.Pull = GPIO_NOPULL ; 
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructx);
}

/**
   * @brief A function used to read and filter accelerometer readings
	 * @param none
   * @param none
	 * @retval 
   */
void get_calibrated_acceleration(void)
{	
	int j;
	
	//Get the accelerometer readings
	LIS3DSH_ReadACC(acceleration_reading);
	
	//Filter Ax, Ay and Az values
	Kalmanfilter_C(acceleration_reading[0], &x_kstate);
	Kalmanfilter_C(acceleration_reading[1], &y_kstate);
	Kalmanfilter_C(acceleration_reading[2], &z_kstate);
	
	//Save the filtered Ax, Ay, Az values
	acceleration_filtered[0] = x_kstate.x;
	acceleration_filtered[1] = y_kstate.x;
	acceleration_filtered[2] = z_kstate.x;
	//acceleration_filtered[3] = 1; //needed for the normalization calculation
	
	//Calculate the normalized acceleration values
	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			acceleration_normalized[i] += acceleration_filtered[j]*calibration_param_matrix[j][i];
		}
		//adding 1*ACC10/20/30
		acceleration_normalized[i] += calibration_param_matrix[3][i];
	}
	
	//Print the filtered values
	//print_filtered_acceleration();
	
	//Calculate the roll and pitch angles
	calculate_angles();
}

/**
   * @brief A function used to print filtered accelerometer readings
	 * @param none
   * @param none
	 * @retval 
   */
void print_filtered_acceleration(void)
{
	
	//Print the accelerometer readings
	//printf("The accelerometer values are\t");
	for(i = 0; i < 3; i++)
	{
		printf("%f,", acceleration_filtered[i]);
	}
	
	printf("\n");
}

/**
   * @brief A function used to print filtered accelerometer readings
	 * @param none
	 * @retval none
   */
void calculate_angles(void)
{
	//pitch = atan2(acceleration_normalized[0], sqrt(acceleration_normalized[1]*acceleration_normalized[1] + acceleration_normalized[2]*acceleration_normalized[2])) * 180/ 3.14159265;
	//roll = atan2(acceleration_normalized[1], sqrt(acceleration_normalized[0]*acceleration_normalized[0] + acceleration_normalized[2]*acceleration_normalized[2])) * 180/ 3.14159265;
	
	//Normalize the angles to show within a 0 to 180 degree range
	//pitch = pitch + 90;
	//roll = roll + 90;
	float float_pitch, float_roll;
	
	float_pitch = atan2(acceleration_normalized[0],acceleration_normalized[2]) * 180/ 3.14159265;
	float_roll = atan2(acceleration_normalized[1],acceleration_normalized[2]) * 180/ 3.14159265;
	roll = (int) fabsf(float_roll); 
	pitch = (int) fabsf(float_pitch);
	
	//Reinitialize the normalized array
	acceleration_normalized[0] =0 ;
	acceleration_normalized[1] =0 ;
	acceleration_normalized[2] =0 ;
}
