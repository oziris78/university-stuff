			PUBLIC n, dizi                  ; disariya degisken belirtme
			EXTRN SIRALIMI:FAR              ; disaridan prosedur ekleme
myss		SEGMENT PARA STACK 'ssss'
			DW 20 DUP(?)
myss		ENDS

myds		SEGMENT PARA 'dddd'
dizi		DB -2, 4, 2, 4, 6, 8, 10, 12, 14
n			DW 9
sonuc		DB 0
myds		ENDS

mycs		SEGMENT PARA 'kkkk'
			ASSUME CS:mycs, DS:myds, SS:myss
ANA			PROC FAR
			PUSH DS
			XOR AX, AX
			PUSH AX
			MOV AX, myds
			MOV DS, AX

			CALL SIRALIMI

			CMP AL, 0
			JZ sirasiz
			MOV sonuc, 1

sirasiz:	RETF
ANA			ENDP
mycs		ENDS
			END ANA