				EXTRN SAYveBOL:NEAR
myss			SEGMENT PARA STACK 'sta'
				DW 20 DUP(?)
myss			ENDS


; 5 elemanli word tanimli isaretli sayilardan olusan bir dizide her 
; bir elemanin binary gosterimindeki 1'lerin sayisini bulup
; dizinin o elemanini buldugunuz sayiya bolen bir asm 
; programini exe tipinde yaziniz, harici yordam kullaniniz

myds			SEGMENT PARA 'dat'
dizi			DW 0FAh, 0FFh, 0ABh, 12h, 16h
n				DW 5	
myds			ENDS

mycs			SEGMENT BYTE PUBLIC 'code'
				ASSUME CS:mycs, DS:myds, SS:myss
MAIN 			PROC FAR
				PUSH DS
				XOR AX, AX
				PUSH AX
				MOV AX, myds
				MOV DS, AX

				XOR SI, SI
				MOV CX, n
L1:				PUSH dizi[SI]
				CALL SAYveBOL
				MOV dizi[SI], AX
				ADD SI, 2
				LOOP L1

				RETF
MAIN			ENDP
mycs			ENDS
				END MAIN