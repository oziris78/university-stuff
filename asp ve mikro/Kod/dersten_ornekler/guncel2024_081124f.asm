		EXTRN SAYveBOL:FAR
myss	SEGMENT PARA STACK 's'
		DW 20 DUP(?)
myss	ENDS
myds	SEGMENT PARA 'data'
dizi	DW 0FEh, 0A7h, 0B3h, 72h, 6Bh
n		DW 5
myds	ENDS
mycs	SEGMENT PARA PUBLIC 'kod'
		ASSUME CS:mycs, DS:myds, SS:myss
ANA		PROC FAR
		PUSH DS
		XOR AX, AX
		PUSH AX
		MOV AX, myds
		MOV DS, AX
		
		MOV CX, n
		LEA DI, dizi
L1:		PUSH [DI]
		CALL SAYveBOL
		MOV [DI], AX
		ADD DI, 2
		LOOP L1
		NOP
		RETF
ANA		ENDP
mycs	ENDS
		END ANA