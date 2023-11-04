STSG SEGMENT PARA STACK 'STSGM'
	DW 20 DUP (?)
STSG ENDS

DTSG SEGMENT PARA 'DTSGM'
	ASAY DW 16
	BSAY DW 20
	GCD DW 0
DTSG ENDS

CDSG SEGMENT PARA 'CDSGM'
	ASSUME CS: CDSG, DS: DTSG, SS: STSG
	
	ANA PROC FAR

		PUSH DS
		XOR AX, AX
		PUSH AX

		MOV AX, DTSG
		MOV DS, AX
		
		MOV AX, ASAY
		MOV BX, BSAY
		CMP AX, BX ; AX<BX ise swap AX<->BX
		JAE NOSWP
		XCHG AX, BX
NOSWP:	
		CMP BX, 0
		JE CIKIS
		XOR DX, DX
		DIV BX ; AX <- DX:AX/BX, DX <- DX:AX % BX
		MOV AX, BX
		MOV BX, DX
		JMP NOSWP
CIKIS:		
		; BX=0
		MOV GCD, AX
		
		RETF
	ANA ENDP


CDSG ENDS

	END ANA