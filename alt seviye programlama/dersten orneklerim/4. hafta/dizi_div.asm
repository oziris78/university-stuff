STEK SEGMENT PARA STACK 'STK'
DW 30 DUP(?)
STEK ENDS

DSG  SEGMENT PARA 'DTS'        
N DW 500
ARRAY DB 1,2,8,20,25,495 DUP(81)
ODD DW 0
DSG ENDS

CSG  SEGMENT PARA 'CODE'
ASSUME CS:CSG, DS:DSG, SS:STEK

BASLA   PROC FAR

PUSH DS
XOR AX,AX
PUSH AX

MOV AX, DSG
MOV DS, AX

;kod buradan sonra
;DIV ornegi

MOV CX, N
LEA BX, ARRAY 
MOV DL, 2

L1: 
MOV AL, [BX]  
XOR AH, AH

DIV DL

CMP AH, 0
JZ L2 ; cift deger
INC ODD
L2:
INC BX

LOOP L1


;kod buraya kadar

RETF
BASLA   ENDP
CSG  ENDS
END BASLA
