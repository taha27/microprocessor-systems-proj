	AREA text, CODE, READONLY
	EXPORT Kalmanfilter_asm
Kalmanfilter_asm
	
	
	;LOADS q,r,x,p From Struct
	;=======================================
	;VLDR.F32 S0, [R2]		;q
	;VLDR.F32 S1, [R2, #4]	;r
	;VLDR.F32 S2, [R2, #8]	;x
	;VLDR.F32 S3, [R2, #12]	;P
								;k is S4
	;faster implementation
	VLDM R2, {S0-S3}
	
	;=======================================
	
loop
	VADD.F32 S3, S3, S0		;p = p + q
	VADD.F32 S5, S3, S1		;temp = p + r
	VDIV.F32 S4, S3, S5		;k = p / p + r
	
	VLDR.F32 S5, [R0]		;Loads measurement value from register
	ADD R0,R0,#4			;increment 4byte offset
	VSUB.F32 S5, S5, S2		;temp = measurement - x
	VMUL.F32 S5, S4, S5		;temp = k * (measurement - x)
	VADD.F32 S2, S2, S5		;x = x + k * (measurement - x)

	VLDR.F32 S5, =1.0 		;temp = 1
	VSUB.F32 S5, S5, S4		;temp = 1 - k
	VMUL.F32 S3, S5, S3 	;p = (1 - k) * p
	
	BVS stop
	
	VSTR.F32 S2, [R1]		;store x value to output array
	ADD	R1,R1, #4	;increment by 4byte offset
	;faster implementation
	VSTM R2,{S0-S3}

	;VSTR.F32 S0, [R2]		;store q
	;VSTR.F32 S1, [R2,#4]	;store r
	;VSTR.F32 S2, [R2,#8]	;store x
	;VSTR.F32 S3, [R2,#12]	;store p
	;VSTR.F32 S4, [R2,#16]	;store k
	SUB R3,R3,#1		;decrements the array length
	CMP R3,#0			;check if size of array is 0
	BNE	loop			;branch to loop
stop
	BX LR
	
	END