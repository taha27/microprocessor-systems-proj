	AREA text, CODE, READONLY
	EXPORT test	
	IMPORT Kalmanfilter_asm

	; r0: address of output array
	; r1: address of input array
	; r2: no. of elements in the array
	; r3: address of kalman filter state

; initialization
Output 	DCFS 		0, 0, 0, 0, 0, 0
				
Input	DCFS 	 	-0.665365, -0.329988, 0.164465, 0.043962, 0.295885, -0.643138
				
Struct	DCFS 		0.1, 0.1, 0, 0.1, 0		; q, r, x, p, k
			
test 
	; argument passing
	LDR 	R0, =Output  		
	LDR		R1, =Input
	LDR 	R2, =6
	LDR 	R3, =Struct
	LDR		R5, =Kalmanfilter_asm
	BX		R5
	
	;S1 = kinit->q = 0.1;
	;S2 = kinit->r = 0.1;
	;S3 = kinit->x = 0.39;
	;S4 = kinit->p = 0.1;
	;S5 = kinit->k = 0;
    
	
	BX		LR
	END