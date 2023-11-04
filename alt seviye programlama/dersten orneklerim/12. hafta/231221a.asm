				EXTRN SAY:FAR

myss			SEGMENT PARA STACK 'sta'
				DW 20 DUP(?)
myss			ENDS

; 300 elemanli word tanimli bir dizide BYTE tanimli bir elemanin kac kez 
; gectigini bulan EXE tipi programi yaziniz.
; Dizinin adresini, eleman sayisini ve aranan sayiyi stack'e 
; atan ve sonucu AX uzerinden donduren harici bir yordam yaziniz
; Kod kesimini ortak alan olarak belirleyecek ek islemleri gerceklestiriniz

myds			SEGMENT PARA 'dat'
dizi			DW 12, 12, 12, 12, 12, 12, 13, 14, 15, 16, 17
n				DW 11
aranan			DB 18
sonuc			DW ?	
; sinavda soyle yazmamiz beklenir:
; dizi			DW 300 DUP(?)
myds			ENDS

mycs			SEGMENT BYTE PUBLIC 'code'  
				ASSUME CS:mycs, DS:myds, SS:myss
MAIN 			PROC FAR
				PUSH DS
				XOR AX, AX
				PUSH AX
				MOV AX, myds
				MOV DS, AX

				PUSH n
				XOR AX, AX			; egerki sayi isaretsiz ise
				MOV AL, aranan
				; MOV AL, aranan	; egerki sayi isaretli ise
				; CBW
				PUSH AX
				LEA AX, dizi
				PUSH AX
				CALL SAY
				MOV sonuc, AX

				RETF
MAIN			ENDP
mycs			ENDS
				END MAIN