STSG SEGMENT PARA STACK 'STSGM'
	DW 20 DUP (?)
STSG ENDS

DTSG SEGMENT PARA 'DTSGM'

DTSG ENDS

CDSG SEGMENT PARA 'CDSGM'
	ASSUME CS: CDSG, DS: DTSG, SS: STSG
	
	ANA PROC FAR

		PUSH DS
		XOR AX, AX
		PUSH AX

		MOV AX, DTSG
		MOV DS, AX
		
		;our code;;;;;;;;;;;;;;;;;;;;;

		; öylesine değer atama yaptım
		XOR DX, DX
		MOV CX, 99

		MOV BL, 20
		MOV CL, 20
		MOV AL, 10

		; asıl kod aşağısı

		CMP BL, CL
		JNZ LAST_LABEL

		CMP AL, CL
		JNS LAST_LABEL
		MOV DX, CX

LAST_LABEL:


		;our code;;;;;;;;;;;;;;;;;;;;;
		
		RETF
	ANA ENDP


CDSG ENDS

	END ANA