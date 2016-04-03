#include <stdio.h>
//#include "arm_math.h"
#include "kalmanfilter.h"

typedef struct kalman_struct{
    float q; // process noise covariance
    float r; // measurement noise covariance
    float x; // estimated value
    float p; // estimation error covariance
    float k; // adaptive kalman filter gain
}kalman_state;

int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int length);
void reset(kalman_state* kinit);
void subtract(float* sub, float* in1, float* in2, int length);
void misc(float* result, float* diff, int length);
void convolve(float* Result, float* in1, float* in2, int length);

float mean(float* input, int length2);
float root(float input);
float squarer(float input);
float correlation(float* in, float* out, int length);

const int length = 10;                          // constant length of array
int i, j;                                       // iterator variables
int int_temp1, int_temp2;                       // temp int variables
float flt_temp1, flt_temp2, flt_temp3;          // temp floating pt variables

float mean_in;
float mean_out;
    
int main(){
    float input[10] = {0.39, 0.50, 0.48, 0.29, 0.25, 0.32, 0.34, 0.48, 0.41, 0.45};
    float output[length];           // array for storing Kalman processed values
    float temp[length];             // array for storing subtraction results
    float miscresult[2] = {0, 0};   // array for storing mean and std dev results
    float holder[length];           // array for storing convolution results


    kalman_state kstate;
    reset(&kstate);
    Kalmanfilter_C(input, output, &kstate, length);
    printf("\n");

    // subtract
    printf("subtraction:\n");
    subtract(temp, input, output, length);
    for(j = 0; j < length; j++){
        printf("%f\n", temp[j]);
    }

    // misc
    printf("\n");
    misc(miscresult, temp, length);
    printf("mean: %f stdev: %f\n", miscresult[0], miscresult[1]);

    // correlation
    printf("correlation: %f\n", correlation(input, output, length));

    // convolution
    printf("\n");
    for(i = 0; i < length; i++){
        holder[i] = 0;
    }
    convolve(holder, input, output, length);
    for(i = 0; i < length + length -1 ; i++){
        printf("convolution[%d]= %f\n", i, holder[i]);
    }

    return 0;
}

void reset(kalman_state* kinit){

    kinit->q = 0.1;
    kinit->r = 0.1;
    kinit->x = 0.39;
    kinit->p = 0.1;
    kinit->k = 0;
}

int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int length){

    int_temp1 = 0;
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

void subtract(float* sub, float* in1, float* in2, int length){

    for(i = 0; i < length; i++){
        sub[i] = in1[i] - in2[i];
    }
}

void misc(float* result, float* diff, int length){

    result[0] = mean(diff, length);
    
    flt_temp1 = 0;
    for(i = 0; i < length; i++){
        flt_temp1 = flt_temp1 + squarer(diff[i] - result[0]);
    }
    
    result[1] = root(flt_temp1 / length);
}

float mean(float* input, int length2){
    
    flt_temp1 = 0;
    for(i = 0; i < length2; i++){
        flt_temp1 = flt_temp1 + input[i];
    }
    return (flt_temp1 / length2);
}

float root(float input){
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

float squarer(float input){
    
    return input * input;
}

float correlation(float* in, float* out, int length){
    
    mean_in = mean(in, length);
    mean_out = mean(out, length);
    
    flt_temp1 = 0;
    flt_temp2 = 0;
    flt_temp3 = 0;
    for(i = 0; i < length; i++){
        flt_temp1 = flt_temp1 + ((in[i] - mean_in) * (out[i] - mean_out));
        flt_temp2 = flt_temp2 + squarer(in[i] - mean_in);
        flt_temp3 = flt_temp3 + squarer(out[i] - mean_out);
    }
    
    return(flt_temp1 / root(flt_temp2 * flt_temp3));
}

void convolve(float* Result, float* in1, float* in2, int length){
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
