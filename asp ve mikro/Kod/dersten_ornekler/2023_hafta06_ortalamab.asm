STSG SEGMENT PARA STACK 'STSGM'
	DW 20 DUP (?)
STSG ENDS

DTSG SEGMENT PARA 'DTSGM'
	DIZI DB 300 DUP (255)
	N DW 300
	ORT DB 0
DTSG ENDS

CDSG SEGMENT PARA 'CDSGM'
	ASSUME CS: CDSG, DS: DTSG, SS: STSG
	
	ANA PROC FAR

		PUSH DS
		XOR AX, AX
		PUSH AX

		MOV AX, DTSG
		MOV DS, AX
		
		MOV CX, N
		XOR BX, BX
		XOR DX, DX
		XOR AX, AX
		CLC
L1:
		ADC AL, DIZI[BX]
		ADC AH, 0
		ADC DX, 0
		INC BX		
		LOOP L1
		DIV BX
		MOV ORT, AL			
		
		RETF
	ANA ENDP


CDSG ENDS

	END ANA