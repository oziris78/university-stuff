				PUBLIC SAYveBOL
mycs			SEGMENT BYTE PUBLIC 'code'
				ASSUME CS:mycs
SAYveBOL		PROC NEAR
				
				PUSH BP
				MOV BP, SP
				PUSH BX
				PUSH CX
				PUSH DX
				
				MOV AX, [BP+4]
				XOR BX, BX
				MOV CX, 16
L1:				SHR AX, 1
				ADC BX, 0
				LOOP L1

				MOV AX, [BP+4]

				CMP BX, 0
				JE sorunvar  ; divide by zero error
				CWD			; AX -> DX:AX
				IDIV BX		; DX:AX / BX -> Bolum: AX, Kalan: DX
				JMP atla
sorunvar:		MOV AX, 0

atla:			POP DX
				POP CX
				POP BX
				POP BP

				RET 2
SAYveBOL		ENDP
mycs			ENDS
				END