my_ss		SEGMENT PARA STACK 'yigin'
			DW 20 DUP(?)
my_ss		ENDS
my_ds		SEGMENT PARA 'veri'
tek_top		DD 0
tek_say		DW 0
tek_ort		DW 0
cift_top	DD 0
cift_say	DW 0
cift_ort	DW 0
n			DW 10
dizi		DW 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
my_ds		ENDS
my_cs		SEGMENT PARA 'kod'
			ASSUME CS:my_cs, DS:my_ds, SS:my_ss
ANA			PROC FAR
			PUSH DS
			XOR AX, AX
			PUSH AX
			MOV AX, my_ds
			MOV DS, AX
			
			MOV CX, n
			LEA SI, dizi
don:		MOV AX, [SI]
			TEST AX, 0001h
			JZ cift
			ADD WORD PTR [tek_top], AX
			ADC WORD PTR [tek_top + 2], 0
			INC tek_say
			JMP ZIPLA
cift:		ADD WORD PTR [cift_top], AX
			ADC WORD PTR [cift_top + 2], 0
			INC cift_say
ZIPLA:		ADD SI, 2
			LOOP don
			
			MOV DX, WORD PTR [cift_top + 2]
			MOV AX, WORD PTR [cift_top]
			DIV cift_say
			MOV cift_ort, AX
			MOV DX, WORD PTR [tek_top + 2]
			MOV AX, WORD PTR [tek_top]
			DIV tek_say
			MOV tek_ort, AX
			
			RETF
ANA			ENDP
my_cs		ENDS
			END ANA