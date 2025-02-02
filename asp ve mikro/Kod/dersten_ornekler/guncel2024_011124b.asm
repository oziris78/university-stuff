myS		SEGMENT PARA 'my'
		ORG 100h
		ASSUME DS:myS, CS:myS, SS:myS
kaynak	PROC NEAR
		XOR BX, BX
		MOV AL, k_esik
		MOV AH, b_esik
don:	CMP dizi[BX], AL
		JB sonraki
		CMP dizi[BX], AH
		JBE bulundu
sonraki:INC BX
		JMP don
bulundu:MOV indis, BL
		RET
kaynak	ENDP
dizi	DB 14, 11, 44, 62, 11, 3, 8, 63, 81, 5, 11
k_esik	DB 15
b_esik	DB 50
indis	DB ?
myS		ENDS
		END kaynak