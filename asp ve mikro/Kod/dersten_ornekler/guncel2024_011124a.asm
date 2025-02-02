ortaksg		SEGMENT PARA 'ortak'
			ORG 100h
			ASSUME DS:ortaksg, CS:ortaksg, SS:ortaksg
Basla:		JMP SICAKLIK
n			DW 10
sicak		DB 10, -29, 57, -3, 34, 56, 23, -12, -15, 1
esik		DB -20
sonuc		DB ?
SICAKLIK	PROC NEAR
			XOR SI, SI
			MOV CX, n
			MOV AL, esik
don:		CMP SI, CX
			JAE bulunamadi
			CMP AL, sicak[SI]
			JG bulundu
			INC SI
			JMP don
bulunamadi:	MOV Sonuc, 0
			JMP bitir
bulundu:	MOV Sonuc, 1
bitir:		RET
SICAKLIK	ENDP
ortaksg		ENDS
			END Basla
