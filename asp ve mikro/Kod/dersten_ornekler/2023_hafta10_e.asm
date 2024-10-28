		INCLUDE 091221.mac                     ; macronumuzu import ediyoruz bir bakıma
cseg	SEGMENT PARA 'tum'
		ORG 100h
		ASSUME CS:cseg, DS:cseg, SS:cseg
		ANA		PROC FAR

		; main code
		XOR SI, SI
		MOV CX, n

L1:		SAR dizi[SI], 1     ;  SHR olmaz çünkü negatifler var SAR olmalı, dizi[SI]'nın yarısını dizi[SI]'ya atar
		INC SI
		LOOP L1
		
		ENKCK dizi, n
		MOV kck, AL
		; main code

		RET
ANA		ENDP

dizi	DB 10, -2, 4, 5, 10, 3, 5, 17
n		DW 8
kck		DB ?

cseg	ENDS
		END ANA