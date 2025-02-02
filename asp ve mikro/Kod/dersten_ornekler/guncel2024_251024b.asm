myds		SEGMENT PARA 'data'
dizi		Db 1, 2, 6, 4, 5
n			DW 5
s			DB ?
myds		ENDS
myss		SEGMENT	PARA STACK 'yig'
			DW 12 DUP(?)
myss		ENDS
mycs		SEGMENT PARA 'kk'
			ASSUME CS:mycs, DS:myds, SS:myss
ANA 		PROC FAR
			PUSH DS
			XOR AX, AX
			PUSH AX
			MOV AX, myds
			MOV DS, AX
			
			MOV BX, 0
			XOR SI, SI
			MOV CX, n
			DEC CX
don:		CMP BX, 0
			JNE son_if
			CMP SI, CX
			JAE son_if
			MOV AL, dizi[SI]
			CMP AL, dizi[SI+1]
			JLE artir
			MOV BX, 1
artir:		INC SI
			JMP don
son_if:		CMP BX, 0
			JNE sirasiz
			MOV s, 1
			JMP Son
sirasiz:	MOV s, 0		
Son:		RETF
ANA			ENDP
mycs		ENDS
			END ANA