

;;;  --------------------------------------------------------  ;;;
;;;  |  BLM3061 - Mikroişlemci Sistemleri ve Assembly Dili  |  ;;;
;;;  |     Lab #2, Oğuzhan Topaloğlu, Ç19052025, Grup2      |  ;;;
;;;  --------------------------------------------------------  ;;;


;; primeOddSum => asal ve tek => 
;; 3-4-5-------> 05
;; 5-12-13-----> 0D
;; 8-15-17-----> 11
;; 9-40-41-----> 29
;; 12-35-37----> 25
;; 20-21-29----> 1D
;; 
;; nonPrimeOrEvenSum => yalnız bir koşul => 			
;; 6-8-10------> 0A
;; 7-24-25-----> 19
;; 9-12-15-----> 0F
;; 10-24-26----> 1A
;; 12-16-20----> 14
;; 14-48-50----> 32
;; 15-36-39----> 27
;; 16-30-34----> 22
;; 18-24-30----> 1E
;; 21-28-35----> 23
;; 24-32-40----> 28
;; 27-36-45----> 2D


SEG_CODE SEGMENT PARA 'kod'
	ORG 100H
	ASSUME CS:SEG_CODE, SS:SEG_CODE, DS:SEG_CODE, ES:SEG_CODE

MY_DATA: JMP MAIN_PROC
; data tanımları başlangıcı
	primeOddSum DB 15 DUP (0)
	nonPrimeOrEvenSum DB 15 DUP (0)

	IS_ODD DB ?       ; tek ise 1, çift ise 0
	IS_PRIME DB ?     ; asal ise 1, değilse 0
	C_SQUARED DW ?    ; temp değişken, c^2 yi tutacak
	C_VALUE DW ?      ; temp değişken, C_SQUARED'in karekökünü tutacak
; data tanımları bitişi

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MAIN_PROC PROC NEAR
	; kodumuzun başlangıcı

	MOV AX, 1                 ; a = 1
A_LOOP:
	MOV BX, AX                ; b = a, 1 yerine a'dan başlatarak optimize ediyorum
B_LOOP:
	; --------------------- loop içinin başlangıcı --------------------- ;
	;;;; a+b tek mi hesapla ve IS_ODD değerini tut
	CALL CALC_IS_ODD

	;;;; C^2 hesapla ve C_SQUARED değerini tut
	CALL CALC_C_SQUARED

	;;;; C_VALUE değerini sqrt alarak tam sayı olarak hesapla
	PUSH AX
	PUSH BX
	PUSH CX
	PUSH DX

	MOV DX, AX                 ; DX = C_SQUARED
	XOR CX, CX

    MOV BX, 1                  ; BX = 1
SQRT_CHECK_LOOP:
	MOV AX, BX                 ; AX = BX
	MUL AX                     ; AX = BX * BX (AX = x * x)
	MOV DX, C_SQUARED          ; DX = c^2
	CMP AX, DX                 ; c^2 = x^2 ise x değerimizdir
	JE SQRT_FOUND              ; Eğer eşitse, sonuç bulundu
	INC BX                     
	CMP BX, 51                 
	JL SQRT_CHECK_LOOP         

    JMP SQRT_NOT_FOUND         ; Eğer tam kare bulunmadıysa buraya atla

SQRT_NOT_FOUND:
	POP DX
	POP CX
	POP BX
	POP AX
	JMP END_OF_LOOP

SQRT_FOUND:
    MOV C_VALUE, BX            ; Karekök sonucunu C_VALUE'ya kaydet
	POP DX
	POP CX
	POP BX
	POP AX

	;;;; Buraya gelindiyse IS_ODD, C_VALUE değişkenleri elimde var demektir
	;;;; IS_PRIME hesabı da yaparsam tüm koşullarım elimde olmuş olur
    CALL CALC_IS_PRIME

    ;;;; IS_PRIME'a göre karar ver
    PUSH AX
    PUSH BX
    
    MOV AH, 0
    MOV BH, 0
    MOV AL, IS_PRIME
    MOV BL, IS_ODD

    CMP AX, 1
    JNE WRONG_CONDITION
    CMP BX, 1
    JNE WRONG_CONDITION
    ; hem asal hem tek sayı o zaman ekle
    CALL CHECK_AND_ADD_TO_primeOddSum
    JMP END_CONDITIONS
WRONG_CONDITION:
    CALL CHECK_AND_ADD_TO_nonPrimeOrEvenSum
END_CONDITIONS:

    POP BX
    POP AX

	; --------------------- loop içinin sonu --------------------- ;
END_OF_LOOP:
	INC BX                    ; b++
	CMP BX, 51
	JL B_LOOP                 ; b <= 50 ise devam

	INC AX                    ; a++
	CMP AX, 51
	JL A_LOOP                 ; a <= 50 ise devam

	; kodumuzun bitişi
	RET
MAIN_PROC ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CHECK_AND_ADD_TO_nonPrimeOrEvenSum PROC NEAR
		PUSH AX
		PUSH BX
		PUSH CX
		PUSH DX

		LEA BX, nonPrimeOrEvenSum
		MOV AX, C_VALUE
		XOR DX, DX
	SEARCH1:
		CMP DX, [BX]
		JE TERM_NOT_FOUND1 ; şu anki terim 0 ise bitti liste demektir
		CMP AX, [BX]
		JE SAME1           ; şu anki terim C_VALUE ile aynı ise ekleme yapılmayacak
		INC BX
		JMP SEARCH1

	TERM_NOT_FOUND1:
		MOV [BX], AL

	SAME1:
		POP DX
		POP CX
		POP BX
		POP AX
		RET
CHECK_AND_ADD_TO_nonPrimeOrEvenSum ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CHECK_AND_ADD_TO_primeOddSum PROC NEAR
		PUSH AX
		PUSH BX
		PUSH CX
		PUSH DX

		LEA BX, primeOddSum
		MOV AX, C_VALUE
		XOR DX, DX
	SEARCH2:
		CMP DX, [BX]
		JE TERM_NOT_FOUND2 ; şu anki terim 0 ise bitti liste demektir
		CMP AX, [BX]
		JE SAME2           ; şu anki terim C_VALUE ile aynı ise ekleme yapılmayacak
		INC BX
		JMP SEARCH2

	TERM_NOT_FOUND2:
		MOV [BX], AL

	SAME2:
		POP DX
		POP CX
		POP BX
		POP AX
		RET
CHECK_AND_ADD_TO_primeOddSum ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CALC_IS_ODD PROC NEAR
		PUSH CX
		PUSH DX
		MOV CX, AX  ; CX = a
		ADD CX, BX  ; CX = a+b

		TEST CX, 1
		JNZ IS_ODD_TRUE
	    MOV IS_ODD, 0
	    JMP IS_ODD_END
	IS_ODD_TRUE:
	    MOV IS_ODD, 1
	IS_ODD_END:
		POP DX
		POP CX
		RET
CALC_IS_ODD ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CALC_IS_PRIME PROC NEAR
		PUSH AX
		PUSH BX
		PUSH CX
		PUSH DX

		MOV AX, C_VALUE            ; Load C_VALUE to AX
		CMP AX, 2                  ; Check if C_VALUE < 2
		JL NOT_PRIME               ; If less than 2, it's not prime
		CMP AX, 2                  ; Check if C_VALUE == 2
		JE IS_PRIME_TRUE           ; If equal to 2, it's prime

		; Check for primes greater than 2
		MOV BX, 2                  ; Start checking from 2
	PRIME_CHECK_LOOP:
		CMP BX, AX                 ; Compare BX with C_VALUE
		JE IS_PRIME_TRUE           ; If equal, it means C_VALUE is prime
		DIV BX                     ; AX / BX, quotient in AX, remainder in DX
		CMP DX, 0                  ; Check remainder
		JE NOT_PRIME               ; If remainder is 0, it's not prime
		INC BX                     ; Move to the next number
		CMP BX, AX                 ; Compare BX with C_VALUE
		JL PRIME_CHECK_LOOP        ; If BX < C_VALUE, continue checking

	IS_PRIME_TRUE:
		MOV IS_PRIME, 1            ; Set IS_PRIME to 1 (true)
		JMP END_PRIME_CHECK

	NOT_PRIME:
		MOV IS_PRIME, 0            ; Set IS_PRIME to 0 (false)

	END_PRIME_CHECK:
		POP DX
		POP CX
		POP BX
		POP AX
		RET
CALC_IS_PRIME ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CALC_C_SQUARED PROC NEAR
	PUSH AX
	PUSH BX
	PUSH CX
	PUSH DX

    MOV DX, 0
    MOV CX, AX 
    MUL AX     
    MOV SI, AX                   ; SI = a*a
    MOV AX, BX
    MUL AX                       ; AX = b*b
    ADD AX, SI                   ; AX = a*a + b*b = c^2
    MOV C_SQUARED, AX
	
	POP DX
	POP CX
	POP BX
	POP AX
	RET
CALC_C_SQUARED ENDP


SEG_CODE ENDS
END MY_DATA

