			PUBLIC ALAN_BUL
mycode		SEGMENT PARA 'kk'
			ASSUME CS:mycode
ALAN_BUL	PROC FAR
			PUSH BP
			PUSH BX
			PUSH CX
			PUSH DI
			PUSH DX

			; burada her 14,16,18 sayilarini bulmadan once kodu yazdik ve yukariya
			; gerekli PUSH komutlarini koyduk, ondan sonra stack'i dusunerek 14,16,18
			; degerlerini bulduk, burada ayrica CALL komutundan gelen offset ve CS degerleri de
			; stack'te bulunmaktadir, FAR oldugu icin iki deger atildi

			; stack durumu (YUKARIDAN ASAGIYA DOGRU, EN ASAGISI EXE KISMINDAN GELIYOR)
			; DX, DI, CX, BX, BP, offset, CS, c, b, a, AX, DS
			; 0,  2,  4,  6,  8,  10,     12, 14,16,18,20, 22
			; demek ki c,b,a sirayla 14, 16, 18'de bulunuyor

			MOV BP, SP
			XOR AX, AX          ; AX = 0
			ADD AX, [BP+14]     ; AX = c
			ADD AX, [BP+16]     ; AX = c+b
			ADD AX, [BP+18]     ; AX = c+b+a
			SHR AX, 1			; u yu bulduk

			MOV BX, AX
			SUB BX, [BP+14]		; u-c

			MOV CX, AX
			SUB CX, [BP+16]		; u-b

			MOV DI, AX
			SUB DI, [BP+18]		; u-a

			MUL BX				; DX:AX <- BX * AX   u*(u-a)
			MUL CX				; AX: u*(u-a)*(u-b)
			MUL DI				; AX: u*(u-a)*(u-b)*(u-c)

			POP DX
			POP DI
			POP CX
			POP BX
			POP BP

			RETF 6     ; burada kalan a,b,c degerlerini (coplerini) stack'ten temizliyoruz
			           ; yoksa EXE program icin en sonda AX ve DS degerlerini cekemeyiz
ALAN_BUL	ENDP
mycode		ENDS
			END