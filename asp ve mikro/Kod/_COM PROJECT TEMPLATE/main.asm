

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;  ŞABLON 1  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SEG_CODE SEGMENT PARA 'kod'
	ORG 100H
	ASSUME CS:SEG_CODE, SS:SEG_CODE, DS:SEG_CODE, ES:SEG_CODE

MAIN_PROC PROC NEAR
	; kodumuzun başlangıcı

	; kodumuzun bitişi
	RET
MAIN_PROC ENDP

; data tanımları başlangıcı

; data tanımları bitişi

SEG_CODE ENDS
END MAIN_PROC


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;  ŞABLON 2  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SEG_CODE SEGMENT PARA 'kod'
	ORG 100H
	ASSUME CS:SEG_CODE, SS:SEG_CODE, DS:SEG_CODE, ES:SEG_CODE

MY_DATA: JMP MAIN_PROC
; data tanımları başlangıcı

; data tanımları bitişi

MAIN_PROC PROC NEAR
	; kodumuzun başlangıcı

	; kodumuzun bitişi
	RET
MAIN_PROC ENDP


SEG_CODE ENDS
END MY_DATA

