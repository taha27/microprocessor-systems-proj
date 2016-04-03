	AREA text, CODE, READONLY
	EXPORT Kalmanfilter_asm	

	; r0: address of output array
	; r1: address of input array
	; r2: no. of elements in the array
	; r3: address of kalman filter state
			
Kalmanfilter_asm 		
	;S1 = kinit->q = 0.1;
	;S2 = kinit->r = 0.1;
	;S3 = kinit->x = 0.39;
	;S4 = kinit->p = 0.1;
	;S5 = kinit->k = 0;
    
	VLDM 		R3, {S1-S5}				; load passed arguments 
	MOV 		R4, #1					;	initialize loop iterator	
	VMOV.F32 	S9, #1					; initialize temp float variable for misc operation

; iterate through input and compute Kalman output
UPDATE	
	VLDR 		S0, [R1]		; update pointer to input array element
	VADD.F32 	S4, S4, S1		; 'p = p + q'			
	VADD.F32 	S6, S4, S2 		; temp placeholder for p + r					
	VDIV.F32 	S5, S4, S6		; 'k = p / (p + r)'						
	VSUB.F32 	S6, S0, S3 		; temp placeholder for 'measurement - x'		
	VNEG.F32 	S8, S5			; temp placeholder for '-k'					
	VMUL.F32 	S7, S5, S6		; temp place holder for 'k * (measurement - x)'
	VADD.F32 	S8, S8, S9		; temp placeholder for '1+(-k)'
	VADD.F32 	S3, S3, S7		; 'x = x + k * (measurement - x)'
	VMUL.F32 	S4, S8, S4		; 'p = (1 - k) * p'
	VSTR 		S3, [R0]		; write output in memory
	ADD 		R0, R0, #4		; increment output address 
	ADD 		R1, R1, #4		; increment input address
	ADD 		R4, #1			; increment loop iterator
	CMP 		R4, R2			; compare loop iterator with specified array size (check if anymore iterations left)
	BLE UPDATE					; loop again if array length is higher than the counter
	
	BX			LR
	END