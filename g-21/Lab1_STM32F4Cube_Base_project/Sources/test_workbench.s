	AREA results, DATA, READWRITE, ALIGN=2
	EXPORT output
	EXPORT struct

struct 	SPACE 	600	;create empty memory for struct and output
output	SPACE	20	
	
	AREA text, CODE, READONLY
	IMPORT Kalmanfilter_asm
	EXPORT test_workbench

input	DCFS	0.1,0.2,0.3,0.4,0.5 ;create input array of size 5
	
test_workbench
;difference between ADR and LDR is that ADR does not perform the data load 	
	ADR R0, input			;set pointer to input array
	LDR R1,=output		;set pointer to filtered array
	LDR R2,=struct		;set pointer to struct
	LDR R3,=5 				;set array length

	;initialize values for q,r,x,p,k
	VLDR.F32 S0, =0.1
	VSTR.F32 S0, [R2]	
	VSTR.F32 S0, [R2,#4]	
	VSTR.F32 S0, [R2,#12]
	VLDR.F32 S0, =0.0
	VSTR.F32 S0, [R2,#8]	

	
	LDR R4, =Kalmanfilter_asm
	BX R4
	
	END