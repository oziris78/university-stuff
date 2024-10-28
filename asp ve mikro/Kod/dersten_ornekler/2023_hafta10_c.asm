		EXTRN TOPLAMA:FAR                    ; 091221c.asm içinde bulunuyor
mys		SEGMENT PARA STACK 's'
		DW 20 DUP(?)
mys		ENDS

myd		SEGMENT PARA 'D'
sayi1	DB 170
sayi2	DB 172
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
		MOV BL, sayi1
		MOV BH, sayi2
		CALL TOPLAMA
		MOV sonuc, AX
		; main code


		RETF
ANA		ENDP

myc		ENDS
		END ANA