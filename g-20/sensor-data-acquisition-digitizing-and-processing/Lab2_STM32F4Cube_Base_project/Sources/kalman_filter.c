/**
  ******************************************************************************
  * File Name          : kalman_filter.c
  * Description        : Implements Kalman filter to eliminate noise from data acquisition
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 11th, 2016
  ******************************************************************************
  */

/* Includes */
#include "kalman_filter.h"
#include "stdint.h"
#include <stdio.h>

/**
	 * @brief This function recursively updates the parameters defined in the Kalman state structure.
	 * The function also checks for NaN output values.
	 * @param InputArray: Double value that corresponds to the noisy input of the Kalman filter
	 * @param OutputArray*: Pointer to a double that corresponds to the filtered output
	 * @param kstate*: Pointer to a kalman_state structure, parametets gets recursively updated
	 * @retval Int value between 0 and 1: 1 meaning the filter output is NaN, 0 meaning the filtered
	 * output value is a valid bounded number
   */
int Kalmanfilter_C(double InputArray, double* OutputArray, kalman_state* kstate){

	/* Update the Kalman state parameters */
	kstate->p = kstate->p + kstate->q;
	kstate->k = kstate->p / (kstate->p + kstate->r);
	kstate->x = kstate->x + kstate->k * (InputArray - kstate->x);
	kstate->p = (1 - kstate->k) * kstate->p;

	OutputArray[0] = kstate->x;
	// printf("Measured position: %f Kalman position: %f\n", InputArray, OutputArray[0]);

	/* Check for NaN */
	if(kstate->x != kstate->x){
		return 1;
	}
	return 0;
}

/**
	 * @brief This function initializes the Kalman filter struct parameters. The initial values are
	 * decided following rigorous experiments that give the best result for our pertinent sensor
	 * @param kinit*: Pointer to a kalman_state structure
   * @retval void
   */
void Reset(kalman_state* kinit){

	kinit->q = 0.014;
	kinit->r = 0.550;
	kinit->x = 0;
	kinit->p = 0.081;
	kinit->k = 0;
}
