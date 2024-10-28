				PUBLIC SAY
				
				; her sey ayni olmalidir ve birlestirmek icin PUBLIC yazmaliyiz
mycs			SEGMENT BYTE PUBLIC 'code'  
				ASSUME CS:mycs
SAY				PROC FAR
				
				PUSH BP
				MOV BP, SP ; BP PUSH'u sonrasinda cat diye calistirdik diye obur PUSH'lardan etkilenmeyiz
				PUSH CX
				PUSH DI
				PUSH BX
				MOV CX, [BP+10]   ; n
				MOV DI, [BP+6]    ; dizi
				MOV BX, [BP+8]    ; aranan

				XOR AX, AX
L1:				
				CMP BX, [DI]
				JNE atla
				INC AX
atla:			
				ADD DI, 2
				LOOP L1

				POP BX
				POP DI
				POP CX
				POP BP
				RETF 6
SAY				ENDP

mycs			ENDS
				END