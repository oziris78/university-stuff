		EXTRN ALAN_B:FAR
myss	SEGMENT PARA STACK 'yigin'
		DW 20 DUP(?)
myss	ENDS
mydata	SEGMENT PARA 'veri'
kenar	DW 6, 8, 5, 9, 4, 8, 2, 2, 3
n		DW 3
enbyk	DW 0
mydata	ENDS
mycode	SEGMENT PARA 'kod'
		ASSUME CS:mycode, SS:myss, DS:mydata
UZAK	PROC FAR
		PUSH DS
		XOR AX, AX
		PUSH AX
		MOV AX, mydata
		MOV DS, AX
		
		XOR SI, SI
		MOV CX, n
L1:		PUSH kenar[SI]
		PUSH kenar[SI+2]
		PUSH kenar[SI+4]
		CALL ALAN_B
		CMP AX, enbyk
		JBE ZIPLA
		MOV enbyk, AX
ZIPLA:	ADD SI, 6
		LOOP L1
		
		RETF
UZAK	ENDP
mycode	ENDS
		END UZAK
