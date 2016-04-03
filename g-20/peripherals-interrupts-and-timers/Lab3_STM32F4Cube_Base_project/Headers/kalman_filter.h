/**
  ******************************************************************************
  * File Name          : kalman_filter.h
  * Description        : Header file for "kalman_filter.c"
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 11th, 2016
  ******************************************************************************
  */
	
/* Global declarations */
typedef struct kalman_struct{
	float q; 		// process noise covariance
	float r; 		// measurement noise covariance
	float x; 		// estimated value
	float p; 		// estimation error covariance
	float k; 		// adaptive kalman filter gain
}kalman_state;

/* Function prototypes */
void Reset(kalman_state* kinit);
int Kalmanfilter_C(float InputArray, float* OutputArray, kalman_state* kstate);
