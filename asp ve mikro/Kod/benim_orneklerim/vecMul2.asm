


PUBLIC VECTOR_MUL

MYCS2 SEGMENT PARA 'MYCS2'
	ASSUME CS:MYCS2

	VECTOR_MUL PROC FAR
		PUSH BP
		MOV BP, SP

		; ( a,b,c ) ve (u, v, w)
		; ( [BP+16],[BP+14],[BP+12] ) ve ( [BP+10],[BP+8],[BP+6] )
		; sonuc: (bw-cv, cu-aw, av-bu)
		; 1 -1 0   7 8 9

		MOV AL, [BP+14]  ; AL = b
		MOV BL, [BP+6]   ; BL = w
		IMUL BL          ; AX = bw
		MOV DX, AX       ; DX = bw

		MOV AL, [BP+12]   ; AL = c
		MOV BL, [BP+8]   ; BL = v
		IMUL BL          ; AX = cv
		SUB DX, AX       ; DX = bw-cv

		PUSH DX


		MOV AL, [BP+12]   ; AL = c
		MOV BL, [BP+10]   ; BL = u
		IMUL BL          ; AX = cu
		MOV DX, AX       ; DX = cu

		MOV AL, [BP+16]  ; AL = a
		MOV BL, [BP+6]   ; BL = w
		IMUL BL          ; AX = aw
		SUB DX, AX       ; DX = cu-aw

		PUSH DX


		MOV AL, [BP+16]  ; AL = a
		MOV BL, [BP+8]   ; BL = v
		IMUL BL          ; AX = av
		MOV DX, AX       ; DX = av

		MOV AL, [BP+14]  ; AL = b
		MOV BL, [BP+10]   ; BL = u
		IMUL BL          ; AX = bu
		SUB DX, AX       ; DX = av-bu

		PUSH DX

		; z y x BP offset CS w v u c b a ...

		POP AX  ; z
		POP BX
		POP DX
		POP BP

		RETF 12
	VECTOR_MUL ENDP

MYCS2 ENDS
	END
