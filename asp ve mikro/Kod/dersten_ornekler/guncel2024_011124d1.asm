		EXTRN TOPLAMA: FAR
myss 	SEGMENT PARA STACK 's'
		DW 20 DUP(?)
myss	ENDS
myds	SEGMENT PARA 'd'
sayi1	DB 17
sayi2	DB 67
top		DW ?
myds	ENDS
mycs	SEGMENT PARA 'k'
		ASSUME CS:mycs, DS:myds, SS:myss
ANA		PROC FAR
		PUSH DS
		XOR AX, AX
		PUSH AX
		MOV AX, myds
		MOV DS, AX
		
		MOV BL, sayi1
		MOV BH, sayi2
		CALL TOPLAMA
		MOV top, AX
		RET
ANA 	ENDP
mycs	ENDS
		END ANA