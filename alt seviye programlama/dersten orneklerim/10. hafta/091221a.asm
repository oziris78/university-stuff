
mys		SEGMENT PARA STACK 's'
		DW 20 DUP(?)
mys		ENDS

myd		SEGMENT PARA 'D'
sayi	DW 2
ust		DW 11
sonuc	DW ?
myd		ENDS

myc		SEGMENT PARA 'K'
		ASSUME CS:myc, DS:myd, SS:mys
ANA		PROC FAR
		PUSH DS
		XOR AX, AX
		PUSH AX
		MOV AX, myd
		MOV DS, AX

		; main code
		MOV CX, ust
		MOV BX, sayi	; BL ve AL kullanırsak taşma olur ve hep 0 * 2 yaparız
		;				; o yüzden BX,AX kullanılmalı
		CALL ustal
		MOV sonuc, AX  ; ustal AX'e atayacak diye düşündük
		; main code

		RETF
ANA		ENDP

ustal	PROC NEAR
		PUSH CX     ; CX'in değerini korumak isteriz, fonksiyonun değiştirmesi kötü olur
		PUSH DX     ; aynı şekilde saklıyoruz çünkü MUL işlemi DX'i değiştirebilir
		MOV AX, 1
L1:		MUL BX      ; AX * BX -> DX:AX
		LOOP L1
		POP DX      ; en son DX'i attık diye ilk onu POP edeceğiz
		POP CX      ; PUSH ile birlikte dönmeden önce eski değerini elde eder
		RET
ustal	ENDP

myc		ENDS
		END ANA