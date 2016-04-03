/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Implementation of Kalman filter and all post-processing logic
	* Author						 : Aditya Saha & Habib Ahmed
	* Version            : 1.0.0
	* Date							 : March 11th, 2016
  ******************************************************************************
  */

/* Includes */
#include <stdio.h>
#include "arm_math.h"

/* Global declarations */
typedef struct kalman_struct{
	float q; // process noise covariance
	float r; // measurement noise covariance
	float x; // estimated value
	float p; // estimation error covariance
	float k; // adaptive kalman filter gain
}kalman_state;

/* Function prototypes 1 */
extern void Kalmanfilter_asm(float* output, float* input, int length, kalman_state* kstate);
int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int length);
void reset(kalman_state* kinit);

/* Function prototypes 2 */
float mean(float* input, int length2);
float root(float input);
float squarer(float input);
void subtract(float* sub, float* in1, float* in2, int length);
void misc(float* result, float* diff, int length);
float correlation(float* in, float* out, int length);
void convolve(float* Result, float* in1, float* in2, int length);

/* Function prototypes 3 */
void arm_sub_f32(float32_t* pSrcA, float32_t* pSrcB, float32_t* pDst, uint32_t blockSize);
void arm_mean_f32(float32_t* pSrc, uint32_t blockSize, float32_t* pResult);
void arm_std_f32(float32_t*	pSrc, uint32_t 	blockSize, float32_t * pResult);	
void arm_correlate_f32(float32_t* pSrcA, uint32_t srcALen, float32_t* pSrcB, uint32_t srcBLen, float32_t*	pDst);
void arm_conv_f32(float32_t* pSrcA, uint32_t srcALen, float32_t* pSrcB, uint32_t srcBLen, float32_t* pDst);

int main(){
	
	const int length = 5;
	float input[length] = {-0.665365, -0.329988, 0.164465, 0.043962, 0.295885};
	
	float output[length];           	// array for storing Kalman processed values
	float temp[length];             	// array for storing subtraction results
	float temp2[length];	
	float miscresult[2] = {0, 0};   	// array for storing mean and std dev results
	float holder[length];           	// array for storing convolution results
	int i, j;
	float corr_temp[1] = {0};

	/*START OF PART II*/
	kalman_state kstate;
	reset(&kstate);
	//Kalmanfilter_C(input, output, &kstate, length);
	Kalmanfilter_asm(output, input, length, &kstate);
	printf("\n");
	/*END OF PART II*/

	/*START OF PART III*/	
	// subtract
	printf("subtraction:\n");
	subtract(temp, input, output, length);
	arm_sub_f32(input, output, temp2, length);
	for(j = 0; j < length; j++){
		printf("My implementation: %f CMSIS: %f\n", temp[j], temp2[j]);
	}

	// misc
	printf("\n");
	//misc(miscresult, temp, length);
	arm_mean_f32(temp, length, &miscresult[0]);
	arm_std_f32(temp, length, &miscresult[1]);
	printf("mean: %f stdev: %f\n", miscresult[0], miscresult[1]);

	// correlation
	//corr_temp[0] = correlation(input, output, length);
	arm_correlate_f32(input, length, output, length, &corr_temp[0]);
	printf("correlation: %f\n", corr_temp[0]);
	

	// convolution
	printf("\n");
	for(i = 0; i < length; i++){
		holder[i] = 0;
	}
	convolve(holder, input, output, length);
	//arm_conv_f32(input, length, output, length, holder);
	for(i = 0; i < length; i++){
		printf("convolution %f \n", holder[i]);
	}
	/*END OF PART III*/

	return 0;
}

/**
	 * @brief Initializes the kalman state struct with set values
	 * @param Kalman state struct
   * @retval void
   */
void reset(kalman_state* kinit){

	kinit->q = 0.1;
	kinit->r = 0.1;
	kinit->x = 0;
	kinit->p = 0.1;
	kinit->k = 0;
}

/**
	 * @brief Updates the kalman struct iteratively upon receiving stream of input data and correspondingly calculating output
	 * @param InputArray- pointer to a floating point variable representing the input data stream
	 * @param OutputArray- pointer to a floating point variable representing the output data stream resulting from the filter
	 * @param kstate- pointer to a kalman_state structure representing the state variable of the filter
	 * @param length- integer number representing the length of the input array
	 * @retval int- integer number representing conversion status of Kalman filter (0 for no error, 1 for error)
   */
int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int length){

	int i;
	int int_temp1 = 0;
	for(i = 0; i < length; i++){
		kstate->p = kstate->p + kstate->q;
		kstate->k = kstate->p / (kstate->p + kstate->r);
		kstate->x = kstate->x + kstate->k * (InputArray[i] - kstate->x);
		kstate->p = (1 - kstate->k) * kstate->p;

		OutputArray[i] = kstate->x;
		printf("Measured position: %f Kalman position: %f\n", InputArray[i], kstate->x);

		if(kstate->x != kstate->x){
			int_temp1++;
		}
	}

	if(int_temp1 > 0){
		printf("Error: NaN!\n");
		return 1;
	}
	return 0;
}

/**
	 * @brief Calculates the arithmetic mean
	 * @param input- pointer to a floating point variable representing the input data string
	 * @param length2- integer number representing the length of the input data string
   * @retval floating point number representing the calculated value for arithmetic mean
   */
float mean(float* input, int length2){
    
	int i;
	float flt_temp1 = 0;
	for(i = 0; i < length2; i++){
		flt_temp1 = flt_temp1 + input[i];
	}
	return (flt_temp1 / length2);
}

/**
	 * @brief Calculates the root
	 * @param input- floating point variable representing the input data 
   * @retval floating point number representing the square root value of the input data
   */
float root(float input){
    
	float flt_temp1;
	int int_temp1, int_temp2, i;
	flt_temp1 = 1;
	int_temp1 = 0;
	int_temp2 = 1000;
	for(i = 0; i < int_temp2; i++){
		int_temp1 = flt_temp1;
		flt_temp1 = (flt_temp1 + input / flt_temp1) / 2;
		if(int_temp1 == flt_temp1){
			break;
		}
	}
	return flt_temp1;
}

/**
	 * @brief Calculates the square of the input data
	 * @param input- floating point variable representing the input data string
   * @retval floating point number representing the calculated value for input squared
   */
float squarer(float input){
    
	return input * input;
}

/**
	 * @brief Calculates the subtraction value for 2 data strings
	 * @param in1/in2- pointers to floating point variables representing 2 input data strings
	 * @param length- integer number representing the length of the input data strings (should be same for both the strings)
   * @retval void
   */
void subtract(float* sub, float* in1, float* in2, int length){
	
	int i;
	for(i = 0; i < length; i++){
		sub[i] = in1[i] - in2[i];
	}
}

/**
	 * @brief Calcualtes the standard deviation of an input data string
	 * @param diff- pointer to a floating point variable representing the input data string
	 * @param result- pointer to a floating point variable representing the output data string
	 * @param length- integer number representing the length of the input data string
   * @retval void
   */
void misc(float* result, float* diff, int length){
    
	float flt_temp1 = 0;
	int i;
	result[0] = mean(diff, length);
	for(i = 0; i < length; i++){
		flt_temp1 = flt_temp1 + squarer(diff[i] - result[0]);
	}

	result[1] = root(flt_temp1 / length - 1);
}

/**
	 * @brief Calcualtes the correlation between two input data strings
	 * @param in/out- pointers to floating point variables representing 2 input data strings
	 * @param length- integer number representing the length of the input data string
   * @retval floating point variable representing the calculated correlation between 2 input data strings
   */
float correlation(float* in, float* out, int length){
    
	float mean_in = mean(in, length);
	float mean_out = mean(out, length);

	float flt_temp1 = 0;
	float flt_temp2 = 0;
	float flt_temp3 = 0;
	int i;
	for(i = 0; i < length; i++){
		flt_temp1 = flt_temp1 + ((in[i] - mean_in) * (out[i] - mean_out));
		flt_temp2 = flt_temp2 + squarer(in[i] - mean_in);
		flt_temp3 = flt_temp3 + squarer(out[i] - mean_out);
	}

	return(flt_temp1 / root(flt_temp2 * flt_temp3));
}

/**
	 * @brief Calcualtes the convolution of two input data strings
	 * @param in1/in2- pointers to floating point variables representing 2 input data strings
	 * @param length- integer number representing the length of the input data string
	 * @param Result- pointer to floating point variable representing the output data sting for storing calculated data
   * @retval void
   */
void convolve(float* Result, float* in1, float* in2, int length){
	
	int i, j, int_temp1, int_temp2;
		for (i = 0; i < (length + length - 1); i++){
		Result[i] = 0;
		int_temp1 = 0;
		int_temp2 = 0;

		int_temp1 = (i >= length - 1) ? i - (length - 1) : 0;
		int_temp2 = (i < length - 1) ? i : length - 1;

		for (j = int_temp1; j <= int_temp2; j++){
			Result[i] += in1[j] * in2[i - j];
		}
	}
}
