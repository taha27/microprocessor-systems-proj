/**
 * @file main.c
 * @author Group21 - Taha Saifuddin, Richard Cheung
 * @date 8 Feb 2016
 * @brief C testbench to implement basic Kalman Filter and other statistical functions
 */

#include <stdio.h>
#include "arm_math.h"
#include "demo.h"

typedef struct stateInfo{
	float q, r, x, p, k;
}kalman_state;

/*FUNCTION DECLARATIONS*/

//Assembly subroutine
extern int Kalmanfilter_asm (float* InputArray, float* OutputArray, kalman_state* kstate, int Length);

//C subroutine
int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int Length);

//Statistical functions
void c_sub(float* vector_one, float* vector_two, float* output_vector, int vector_length);
void c_mean(float* input_vector, int vector_length, float* vector_mean);
void c_std(float* input_vector, int vector_length, float* vector_std);
void c_correlate(float* vector_one, float* vector_two, float* vector_correlation, int vector_length);
void c_conv(float* vector_one, float* vector_two, float* vector_convolution, int vector_length);

//Functions to check for correctness
void is_valid(float *ScrA, float *ScrB, int vector_length, float tol, char *errorCode);
void is_valid_relative(float *ScrA, float *ScrB, int vector_length, float errTol, float errRelativeTol, char *errorCode);

/*FUNCTION IMPLEMENTATIONS*/

//The following method takes in two vectors and their length, and calculates their corresponding difference vector 
void c_sub(float* vector_one, float* vector_two, float* output_vector, int vector_length)
{
	int i;
	for(i = 0; i < vector_length; i++)
	{
		output_vector[i] = vector_one[i] - vector_two[i];
	}
}

//The following method takes an input vector and its length, and then finds its mean
void c_mean(float* input_vector, int vector_length, float* vector_mean)
{
	int i;
	float sum = 0;
	for(i = 0; i < vector_length; i++) 
	{
		sum = sum + input_vector[i];
	}

	*vector_mean = sum / vector_length;
}

//The following method takes an input vector and its length, and calculates its standard deviation
void c_std(float* input_vector, int vector_length, float* vector_std)
{	
	float sumOfSquares = 0;
	float sum = 0;
	int i = 0;
	while (i < vector_length) {
		sumOfSquares += input_vector[i] * input_vector[i];
		sum += input_vector[i];
		i++;
	}
	arm_sqrt_f32(((sumOfSquares - (sum * sum) / vector_length) / (vector_length - 1)), vector_std);
}

//The following method takes two vectors as input and their length, and then calculates their correlation
void c_correlate(float* vector_one, float* vector_two, float* vector_correlation, int vector_length)
{
	int correlation_length = vector_length + vector_length - 1;
	int i, kmin, kmax, j;
	for(i = 0; i < correlation_length; i++) 
	{
		vector_correlation[i]=0; 
		//determine the bounds
		kmin = (i <= vector_length - 1) ? 0 : i - vector_length+1;
		kmax = (i < vector_length - 1) ? i : vector_length - 1;
		
		for(j = kmin; j <= kmax; j++) 
		{
			vector_correlation[i] += vector_one[j] * vector_two [(vector_length - (i -j) - 1 ) ];
		}
	}
}

//The following method takes two vectors as input and their length, and then calculates their convolution
void c_conv(float* vector_one, float* vector_two, float* vector_convolution, int vector_length)
{	
	int convolution_length = vector_length + vector_length - 1;
	int i, kmin, kmax, j;
	for(i = 0; i < convolution_length; i++) 
	{
		vector_convolution[i]=0; 
		
    kmin = (i >= vector_length - 1) ? i - (vector_length - 1) : 0;
    kmax = (i < vector_length - 1) ? i : vector_length - 1;
		
		for(j = kmin; j <= kmax; j++) 
		{
			vector_convolution[i] += vector_one[j] * vector_two [i - j];
		}
	}
}

//The following method takes an array of measurements and filters the values using the Kalman Filter
int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int Length)
{	
	int i;
	for(i = 0; i < Length; i++){
		kstate->p = kstate->p + kstate->q;
		kstate->k = kstate->p / (kstate->p + kstate->r);
		kstate->x = kstate->x + kstate->k * (InputArray[i]-kstate->x);

		//The following conditional checks if kstate->x is not a number
		if (kstate->x != kstate->x) {
			printf("An error has occured -NaN value was found"); 
			return 1;
		}

		kstate->p = (1-kstate->k) * kstate->p;

		//Add the filtered value to the output array
		OutputArray[i] = kstate->x;
	}
	return 0; 
}

//The following method takes two float arrays as inputs and compares them to each other to see if they match within a given absolute error
void is_valid(float *ScrA, float *ScrB, int vector_length, float tol, char *errorCode)
{
	int failed = 1;
	for(int i = 0; i < vector_length; i++)
	{
		if(ScrA[i] - ScrB[i] > tol || ScrA[i] - ScrB[i] < -tol)
		{
			failed = 0;
			printf("Tests fails at %s with absolute error tolerance %f:<SrcA:%f, SrcB:%f>\n", errorCode, tol,ScrA[i], ScrB[i]);
		}
		printf("Tests Succeed at %s with absolute error tolerance %f\n", errorCode, tol);

	}
	if (failed) 
	{
		printf("Tests Succeed at %s with absolute error tolerance %f\n", errorCode, tol);
	}
}

//The following method takes two float arrays as inputs and compares them to each other to see if they match within a given relative error
void is_valid_relative(float *ScrA, float *ScrB, int vector_length, float errTol, float errRelativeTol, char *errorCode)
{
	int failed = 1;
	for(int i = 0; i < vector_length; i++)
	{
		float relativeError; 
		if(ScrA[i] - ScrB[i] > errTol || ScrA[i] - ScrB[i] < -errTol)
		{
			failed = 0;
			printf("Relative Tests fails at %s :<SrcA:%f, SrcB:%f>\n", errorCode, ScrA[i], ScrB[i]);
		}

		relativeError= (ScrB[i] > ScrA[i]) ? fabsf(ScrB[i]-ScrA[i]) / ScrB[i] : fabsf(ScrA[i]-ScrB[i]) / ScrA[i];

		failed = (relativeError <= errRelativeTol) ? 1:0;
	}
	if (failed) 
	{
		printf("Relative Tests Succeed" );
	}
}


int main()
{
	//initialize testing array
	float testVector[] = {0.1f,0.2f,0.3f,0.4f,0.5f};
	
	/*COMMENTED OUT LENGTH PARAM AS IT IS INCLUDED IN HEADER FILE*/
	//get the size of the array
	//int length = sizeof(testVector)/sizeof(float);

	//initiate empty output array of size length
	float outputArrayC[length];
	//initialize the struct at p=r=q 0.1 and x=k=0 
	kalman_state currentState = {0.1f, 0.1f, 0.0f , 0.1f, 0.0f};
	//call function Kalmanfilter_C
	Kalmanfilter_C(measurements, outputArrayC, &currentState, length);
	
	//initiate empty output array of size length
	float outputArrayASM[length];
	
	//reinitialize the struct at p=r=q 0.1 and x=k=0 
	currentState.p = DEF_p; 
	currentState.r = DEF_r; 	
	currentState.k = DEF_k;
	currentState.q = DEF_q; 
	currentState.x = DEF_x; 
	
	//call subroutine Kalmanfilter_asm
	Kalmanfilter_asm(measurements, outputArrayASM, &currentState, length );

	//Check for correctness with a error tolerance of 0.000001
	float errorTolerance = 0.000001f;
	float errorPercentage = 0.01;

	//is_valid(outputArrayC, outputArrayASM, length, errorTolerance, "c vs asm");
	//is_valid_relative(outputArrayC, outputArrayASM, length, errorTolerance, errorPercentage,"c vs asm");

	int p;

	//print KalmanFilter output
	for ( p = 0; p < length;  p++ )
	{
		printf("OutputASM: %f & OutputC %f\n", outputArrayASM[p], outputArrayC[p]); 
	}

	float differenceC[length];
	float differenceCMSIS[length];
	
	//Difference
	arm_sub_f32 (measurements, outputArrayC, differenceCMSIS, length);
	c_sub(measurements, outputArrayC, differenceC, length);
	
	//is_valid(differenceC, differenceCMSIS, length, errorTolerance, "Difference");
	//is_valid_relative(differenceC, differenceCMSIS, length, errorTolerance, errorPercentage,"Difference");
	
	//Print difference vector
	for ( p = 0; p < length;  p++ )
	{
		printf("DifferenceC: %f & DifferenceCMSIS %f \n", differenceC[p], differenceCMSIS[p]); 
	}


	//Mean
	float meanCMSIS;
	float meanC; 
	arm_mean_f32 (differenceCMSIS, length , &meanCMSIS);
	c_mean(differenceC,length, &meanC); 
	//is_valid(&meanC, &meanCMSIS, 1, errorTolerance, "mean"); 
	//is_valid_relative(&meanC, &meanCMSIS, 1, errorTolerance, errorPercentage, "mean"); 
	
	//Print mean values
	printf("MeanC: %f & MeanCMSIS %f \n", meanC, meanCMSIS); 
	
	//STD
	float stdC;
	float stdCMSIS;
	arm_std_f32 (differenceCMSIS, length, &stdCMSIS);
	c_std(differenceC, length, &stdC);
	//is_valid(&stdC, &stdCMSIS, 1, errorTolerance, "STD");
	//is_valid_relative(&stdC, &stdCMSIS, 1, errorTolerance, errorPercentage,"STD");

	//Print std values
	printf("StandardDevC: %f & StandardDevCMSIS %f \n", stdC, stdCMSIS); 

	//correlation
	float corC[2*length-1];
	float corCMSIS[2*length-1];
	arm_correlate_f32 (measurements, length, outputArrayC, length, corCMSIS);
	c_correlate(measurements, outputArrayC, corC, length);
	//is_valid(corC, corCMSIS, 2*length-1, errorTolerance, "correlation"); 
	//is_valid_relative(corC, corCMSIS, 2*length-1, errorTolerance, errorPercentage, "correlation"); 
	
	//convolution
	float convC[2*length-1];
	float convCMSIS[2*length-1];
	arm_conv_f32 (measurements, length, outputArrayC, length, convCMSIS);
	c_conv(measurements, outputArrayC, convC, length);
	//is_valid(convC, convCMSIS, 2*length-1, errorTolerance, "convolution"); 
	//is_valid_relative(convC, convCMSIS, 2*length-1, errorTolerance, errorPercentage, "convolution"); 

	//Print correlation and convolution values
	for ( p = 0; p < (2*length-1);  p++ )
	{
		printf("ConvC: %f & ConvCMSIS: %f \n", convC[p], convCMSIS[p]); 
	}
	for ( p = 0; p < (2*length-1);  p++ )
	{
		printf("CorrelateC: %f & CorrelatCMSIS: %f \n", corC[p], corCMSIS[p]); 
	}

	return 0;
}
