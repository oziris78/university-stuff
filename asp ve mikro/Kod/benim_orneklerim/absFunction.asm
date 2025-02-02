

STSG SEGMENT PARA STACK 'STSGM'
	DW 20 DUP (?)
STSG ENDS

DTSG SEGMENT PARA 'DTSGM'
	X DW -2
	F_X DW ?
DTSG ENDS

CDSG SEGMENT PARA 'CDSGM'
	ASSUME CS: CDSG, DS: DTSG, SS: STSG
	
	ANA PROC FAR

		PUSH DS
		XOR AX, AX
		PUSH AX

		MOV AX, DTSG
		MOV DS, AX
		
		; our code

		; doğrudan atama yap
		MOV AX, X
		MOV F_X, AX

		CMP X, 0
		JNS NOTNEGATIVE ; x >= 0 ise aşağıya gitme
		; X negatif ise buraya gelir, negate ederiz
		MOV AX, F_X
		NEG AX
		MOV F_X, AX

NOTNEGATIVE:


		; our code


		
		RETF
	ANA ENDP


CDSG ENDS

	END ANA


