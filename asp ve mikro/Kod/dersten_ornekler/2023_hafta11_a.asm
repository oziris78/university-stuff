			EXTRN ALAN_BUL:FAR               ; dosyada olmayan proseduru belirt
myss		SEGMENT PARA STACK 'ssss'
			DW 20 DUP(?)
myss		ENDS

; ardisik 3 sayi ile ucgenlerin kenar uzunluklarini veriyoruz
; bu ucgenlerin alanlarinin karelerini bulup en buyuk alan^2 yi enbykalan degiskenine ata
; n -> kac adet ucgen bulundugu sayisi
; heron formulu:  alan^2 = u*(u-a)*(u-b)*(u-c)

myds		SEGMENT PARA 'dddd'
kenarlar	DW 8, 6, 5, 9, 12, 7, 2, 3, 2
n			DW 3
enbykalan	DW 0
myds		ENDS

mycs		SEGMENT PARA 'kkkk'
			ASSUME CS:mycs, DS:myds, SS:myss
ANA			PROC FAR
			PUSH DS
			XOR AX, AX
			PUSH AX
			MOV AX, myds
			MOV DS, AX

			; main code
			MOV CX, n
			XOR BX, BX

			; her ucgen icin degerleri stack'e atarak ALAN_BUL fonksiyonunu kullan
			; sonra da ALAN_BUL AX uzerinden sana deger donsun bunu kullanarak
			; karsilastirma yap
L1:			
			PUSH kenarlar[BX]
			PUSH kenarlar[BX+2]
			PUSH kenarlar[BX+4]

			CALL ALAN_BUL

			CMP AX, enbykalan
			JBE devam
			MOV enbykalan, AX
devam:		ADD BX, 6
			LOOP L1
			; main code

			RETF
ANA			ENDP
mycs		ENDS
			END ANA