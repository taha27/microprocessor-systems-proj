/**
  ******************************************************************************
  * File Name          : 
  * Description        : 
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 11th, 2016
  ******************************************************************************
  */
	
/* Includes */
#include "stdint.h"
#include <stdio.h>

typedef struct kalman_struct{
	double q; 		// process noise covariance
	double r; 		// measurement noise covariance
	double x; 		// estimated value
	double p; 		// estimation error covariance
	double k; 		// adaptive kalman filter gain
}kalman_state;

/* Function prototypes */
void Reset(kalman_state* kinit);
int Kalmanfilter_C(double InputArray, double* OutputArray, kalman_state* kstate);
