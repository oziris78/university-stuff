
PUBLIC DIST_SQUARED_TO_AX

myseg SEGMENT PARA 'seg2'
	ASSUME CS:myseg

DIST_SQUARED_TO_AX PROC FAR
	
	PUSH BP
	PUSH BX
	PUSH DX
	PUSH AX

	; asagidan yukariya dogru
	; DS, AX, x1, y1, x2, y2, CS, IP, BP, BX, DX, AX
	; 22, 20, 18, 16, 14, 12, 10, 8,  6,  4,  2,  0
	; 

	MOV BP, SP

	; p1 = (BL, BH) = (x1, y1)
	MOV BL, [BP+18]  ; x1
	MOV BH, [BP+16]  ; y1

	; p2 = (DL, DH) = (x2, y2)
	MOV DL, [BP+14]  ; x2
	MOV DH, [BP+12]  ; y2
	

	SUB BL, DL    ; BL = x1 - x2
	SUB BH, DH    ; BH = y1 - y2

	XOR AX, AX    ; AX = 0
	MOV AL, BL    ; AL = BL
	MUL BL       ; AX <- AL * BL = BL * BL
	MOV DX, AX
 
	XOR AX, AX    ; AX = 0
	MOV AL, BH    ; AL = BH
	MUL BH       ; AX <- AL * BH = BH * BH

	ADD AX, DX

	POP AX
	POP DX
	POP BX
	POP BP

	RETF 8  ; 4 deger cekmek istiyorum o yuzden 8 byte cek

DIST_SQUARED_TO_AX ENDP
	
myseg ENDS
	END
