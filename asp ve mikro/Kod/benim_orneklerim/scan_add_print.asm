

STSG SEGMENT PARA STACK 'STSGM'
	DW 32 DUP (?)
STSG ENDS



DTSG	SEGMENT PARA 'DATA'
CR	EQU 13
LF	EQU 10
MSG1	DB 'ilk sayiyi giriniz: ', 0
MSG2	DB CR, LF, 'ikinci sayiyi giriniz: ', 0
HATA	DB CR, LF, 'Sayi girmelisiniz, yeniden giris yapiniz!', 0
SONUC	DB CR, LF, 'Toplam = ', 0
SAYI1	DW ?
SAYI2	DW ?
DTSG 	ENDS 



CDSG SEGMENT PARA 'CDSGM'
	ASSUME CS: CDSG, DS: DTSG, SS: STSG
	

	; --------------- MAIN PROSEDUR ---------------
	MAIN PROC FAR 
 
		PUSH DS
		XOR AX, AX
		PUSH AX

		MOV AX, DTSG
		MOV DS, AX
		
		; ------------------- ASIL KOD -------------------

		MOV AX, OFFSET MSG1
		CALL PUT_STR           ; msg1'i goster
		CALL GETN              ; sayi1'i oku
		MOV SAYI1, AX

		MOV AX, OFFSET MSG2
		CALL PUT_STR           ; msg2'i goster
		CALL GETN              ; sayi2'i oku
		MOV SAYI2, AX

		MOV AX, OFFSET SONUC
		CALL PUT_STR           ; sonuc'u goster
		
		MOV AX, SAYI1
		ADD AX, SAYI2
		CALL PUTN              ; toplam'i yaz

		; ------------------- ASIL KOD -------------------

		RETF
	MAIN ENDP




	; --------------- GETC PROSEDUR ---------------
	; klavyeden basilan karakteri AL'ye atar, yalnizca AL etkilenir
	;
	GETC PROC NEAR
		MOV AH, 1H
		INT 21H
		RET
	GETC ENDP




	; --------------- PUTC PROSEDUR ---------------
	; AL yazmacindaki degeri ekranda gosterir
	; DL ve AH degisiyor diye AX ve DX yazmaclarini saklar
	;
	PUTC PROC NEAR
		PUSH AX
		PUSH DX
		MOV DL,AL
		MOV AH,2
		INT 21H
		POP DX
		POP AX
		RET
	PUTC ENDP



	; --------------- GETN PROSEDUR ---------------
	; klavyeden basilan sayiyi okur, sonucu AX uzerine koyar, BX,CX,DX yazmaclari saklanir yalnizca AX etkilenir
	; DX: sayinin isaretli olup olmadigini belirler, 1 (+), -1 (-) demek
	; BL: hane bilgisini tutar
	; CX: okunan sayinin islenmesi sirasinda ara degerleri tutar
	; AL: klavyeden okunan karakteri ASCII olarak tutar
	; AX: donus degeri durumundadir
	;
	GETN PROC NEAR
		PUSH BX
		PUSH CX
		PUSH DX
		
	GETN_START:
		MOV DX,1
		XOR BX,BX
		XOR CX,CX
	NEW:
		CALL GETC
		CMP AL,CR
		JE FIN_READ
		CMP AL, '-'
		JNE CTRL_NUM
	NEGATIVE:
		MOV DX, -1
		JMP NEW
	CTRL_NUM:
		CMP AL, '0'
		JB ERROR
		CMP AL, '9'       ; '3' yaparsam 1,2,3 alir yalnizca
		JA ERROR
		SUB AL,'0'
		MOV BL,AL
		MOV AX,10
		PUSH DX 
		MUL CX
		POP DX
		MOV CX,AX
		ADD CX,BX
		JMP NEW
	ERROR:
		MOV AX, OFFSET HATA
		CALL PUT_STR
		JMP GETN_START
	FIN_READ:
		MOV AX,CX
		CMP DX,1
		JE FIN_GETN
		NEG AX
	FIN_GETN:
		POP DX
		POP CX
		POP BX
		RET
	GETN ENDP




	; --------------- PUTN PROSEDUR ---------------
	; AX'de bulunan sayiyi onluk tabanda hane hane yazdirir, CX ve DX saklanir
	; CX: haneleri 10'a bolmeye yarar, CX = 10 olacak
	; DX: 32 bolmede isleme dahil olacak, sonucu etkilemesin diye 0 olmali
	;
	PUTN PROC NEAR
		PUSH CX
		PUSH DX
		XOR DX, DX
		PUSH DX
		MOV CX, 10
		CMP AX,0
		JGE CALC_DIGITS
		NEG AX
		PUSH AX
		MOV AL, '-'
		CALL PUTC
		POP AX
	CALC_DIGITS:
		DIV CX
		ADD DX,'0'
		PUSH DX
		XOR DX,DX
		CMP AX,0
		JNE CALC_DIGITS
	DISP_LOOP:
		POP AX
		CMP AX,0
		JE END_DISP_LOOP
		CALL PUTC
		JMP DISP_LOOP
	END_DISP_LOOP:
		POP DX
		POP CX
		RET
	PUTN ENDP




	; --------------- PUT_STR PROSEDUR ---------------
	; AX'de adresi verilen sonunda 0 olan dizgeyi karakter karakter yazdirir
	; BX: dizgiye indis olarak kullanilir, onceki degeri saklanmalidir
	;
	PUT_STR PROC NEAR
		PUSH BX
		MOV BX, AX
		MOV AL, BYTE PTR [BX]
	PUT_LOOP:
		CMP AL,0
		JE PUT_FIN
		CALL PUTC
		INC BX
		MOV AL, BYTE PTR [BX]
		JMP PUT_LOOP
	PUT_FIN:
		POP BX
		RET
	PUT_STR ENDP



CDSG ENDS

	END MAIN

