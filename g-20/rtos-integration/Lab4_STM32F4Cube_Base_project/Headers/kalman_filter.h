/**
  ******************************************************************************
  * File Name          : kalman_filter.h
  * Description        : Header file for "kalman_filter.c"
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 18th, 2016
  ******************************************************************************
  */
	
/* Global declarations */
typedef struct kalman_struct{
	double q; 		// process noise covariance
	double r; 		// measurement noise covariance
	double x; 		// estimated value
	double p; 		// estimation error covariance
	double k; 		// adaptive kalman filter gain
}kalman_state;

/* Function prototypes */
void Reset_ADC(kalman_state* kinit);
void Reset_MEMS(kalman_state* kinit);
int Kalmanfilter_C(double InputArray, double* OutputArray, kalman_state* kstate);
