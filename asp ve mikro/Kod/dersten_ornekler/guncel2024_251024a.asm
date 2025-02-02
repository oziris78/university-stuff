datasg		SEGMENT PARA 'veri'
a			DB 13
b			DB 11
c 			DB 12
tip 		DB ?
datasg		ENDS
stacksg		SEGMENT PARA STACK 'yigin'
			DW 12 DUP(?)
stacksg		ENDS
codesg		SEGMENT PARA 'kod'
			ASSUME DS:datasg, SS:stacksg, CS:codesg
UCGEN		PROC FAR
			PUSH DS
			XOR AX, AX
			PUSH AX
			MOV AX, datasg
			MOV DS, AX
			
			MOV AL, a
			MOV BL, b
			MOV CL, c
			CMP AL, BL
			JE J1
			CMP BL, CL
			JE J2
			CMP AL, CL
			JE J2
			MOV tip, 3
			JMP JSon
J1:			CMP AL, CL
			JNE J2
			MOV tip, 1
			JMP JSon
J2:			MOV tip, 2
JSon:		RETF
UCGEN		ENDP
codesg		ENDS
			END UCGEN
			
			