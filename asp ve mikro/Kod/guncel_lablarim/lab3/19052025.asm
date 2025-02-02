


;;;  --------------------------------------------------------  ;;;
;;;  |  BLM3061 - Mikroişlemci Sistemleri ve Assembly Dili  |  ;;;
;;;  |     Lab #3, Oğuzhan Topaloğlu, Ç19052025, Grup2      |  ;;;
;;;  --------------------------------------------------------  ;;;


GIRIS_DIZI MACRO
    LOCAL L1
    ; parametre aktarımını sağlamak
    POP CX       ; CX = SAYILAR_LEN
    POP BX       ; BX = SAYILAR

    PUSH AX ; eski değeri saklamak

 L1:
    MOV AX, OFFSET STR_BIR_SAYI
    CALL PRINT_STRING

    CALL SCAN_NUMBER
    MOV [BX], AL      ; diziye kaydet
    ADD BX, 2         ; dizi "pointer"ını arttır
    LOOP L1

    POP AX
ENDM



SEG_STACK SEGMENT PARA STACK 'yigin'
    DW 32 DUP (?)
SEG_STACK ENDS



SEG_DATA SEGMENT PARA 'veri'
    CR EQU 13
    LF EQU 10

    ; nümerik veriler
    MOD_VALUE    DW -1          ; program sonunda hesaplanacak mod değeri
    TEMP         DW -1          ; hesaplamayı kolaylaştırmak için
    SAYILAR_LEN  DW 0           ; SAYILAR dizisinin dinamik, kullanıcıdan girilen boyutu
    SAYILAR      DW 10 DUP(0)
 
    ; info stringleri
    STR_KAC_SAYI    DB CR, LF, 'Kac adet sayi gireceksiniz [1-10]: ', 0
    STR_BIR_SAYI    DB CR, LF, 'Diziye [0, 255] arasi sayi ekleyiniz: ', 0
    STR_SONUC_MOD   DB CR, LF, '***** Girdiginiz dizinin modu: ', 0

    ; error stringleri
    STR_ERR_LARGE         DB CR, LF, 'Girdiginiz sayi 10dan buyuk olmamali!', 0
    STR_ERR_SMALL         DB CR, LF, 'Girdiginiz sayi 1den kucuk olmamali!', 0
    STR_ERR_SCAN_NUMBER   DB CR, LF, 'SCAN_NUMBER proseduru hata verdi!', 0
SEG_DATA ENDS



SEG_CODE SEGMENT PARA 'kod'
    ASSUME CS: SEG_CODE, DS: SEG_DATA, SS: SEG_STACK
    
    MAIN_PROC PROC FAR
        PUSH DS
        XOR AX, AX
        PUSH AX
        MOV AX, SEG_DATA
        MOV DS, AX
        
        ; ---------------------  kodumuzun başlangıcı  --------------------- ;
        ;;;; SAYILAR_LEN okuması yap
        CALL READ_SAYILAR_LEN

        ;;;; **MAKRO KULLANARAK** diziyi oku ve kaydet
        LEA AX, SAYILAR
        PUSH AX 
        MOV AX, SAYILAR_LEN
        PUSH AX 
        GIRIS_DIZI

        ;;;; MOD isimli altyordam ile mod hesapla
        MOV AX, SAYILAR_LEN
        PUSH AX
        LEA AX, SAYILAR
        PUSH AX
        CALL MOD_PROC
        POP AX
        POP AX

        ;;;; Sonucu yazdırma
        MOV AX, OFFSET STR_SONUC_MOD
        CALL PRINT_STRING
        MOV AX, MOD_VALUE
        CALL PRINT_NUMBER

        ; ---------------------  kodumuzun bitişi  --------------------- ;
        
        RETF
    MAIN_PROC ENDP


; -------------------------------------------------------- ;
; stackten veri alıp modları hesaplayıp aşağıdaki yazdığım 
;     C kodunu implemente eden prosedür:
;
; int main() {
;     int arr[] = { 1, 1, 2, 3, 4 };
;     int N = sizeof(arr) / sizeof(arr[0]);
;     int mode_count = -1, mod_value = -1;
; 
;     for (int i = 0; i < N; i++) {
;         int x = arr[i];
;         int count = 0;
; 
;         for (int j = 0; j < N; j++) {
;             if (x == arr[j]) count++;
;         }
; 
;         if (count > mode_count) {
;             mode_count = count;
;             mod_value = x;
;         }
;     }
; 
;     // print mod_value, mode_count
; 
;     return 0;
; }

; STACK =>  SAYILAR_LEN, SAYILAR, <call>, BP
;                +6         +4         
; https://stackoverflow.com/a/44986023
MOD_PROC PROC NEAR
    PUSH BP
    MOV BP, SP
    ; register değerlerini sakla
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    PUSH DI

    ; stack'ten verileri okuma:
    ; SAYILAR      =>  [BP+4]
    ; SAYILAR_LEN  =>  [BP+6]

    XOR CX, CX                      ; CX = i (outer loop)

LB_OUTER_LOOP:
    CMP CX, [BP+6]                  ; i =? N
    JE LB_END_OUTER_LOOP

    ; --------- for_i başlangıç
    MOV BX, [BP+4]                  ; BX = arr
    MOV AX, CX                      ; AX = i
    SHL AX, 1                       ; AX = i * 2
    ADD BX, AX                      ; BX = arr + i * 2
    MOV BX, WORD PTR [BX]           ; BX = arr[i]

    XOR SI, SI                      ; SI = count = 0

    MOV AX, 0                       ; DX = j
LB_INNER_LOOP:
    CMP AX, [BP+6]                  ; j =? N
    JE LB_END_INNER_LOOP

    MOV DI, [BP+4]                  ; DI = arr
    PUSH BX
        MOV BX, AX                  ; BX = j
        SHL BX, 1                   ; BX = j * 2
        ADD DI, BX                  ; DI = arr + j * 2
    POP BX
    MOV DI, WORD PTR [DI]           ; DI = arr[j]

    CMP DI, BX                      ; arr[i] ==? arr[j]
    JNE LB_NOT_SAME
    INC SI                          ; count++
LB_NOT_SAME:
    INC AX                          ; j++
    JMP LB_INNER_LOOP

LB_END_INNER_LOOP:
    MOV AX, TEMP
    CMP SI, AX
    JNG LB_NOT_NEW_MODE   

    ; yeni değerlerin saklanması
    MOV TEMP, SI
    MOV MOD_VALUE, BX

LB_NOT_NEW_MODE:
    INC CX                          ; i++
    JMP LB_OUTER_LOOP

LB_END_OUTER_LOOP:
    ; register değerlerini geri al
    POP DI
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    POP BP
    RET
MOD_PROC ENDP




; -------------------------------------------------------- ;
; SAYILAR_LEN değişkenini kullanıcıdan okur ve depolar
; bu prosedür ödevde istenilmedi, ben yardımcı olsun diye yazdım
; dizinin okunması ödevde istenildiği gibi makro ile yapılıyor!
READ_SAYILAR_LEN PROC NEAR
    PUSH AX

KAC_SAYI_START:
    MOV AX, OFFSET STR_KAC_SAYI
    CALL PRINT_STRING

    CALL SCAN_NUMBER                ; AX = girdi
    CMP AX, 1
    JL INVALID_KAC_SAYI_SMALL       ; AX < 1 ise hata
    CMP AX, 10
    JG INVALID_KAC_SAYI_LARGE       ; AX > 10 ise hata
    JMP KAC_SAYI_END                ; buraya gelirse [1, 10] içinde
INVALID_KAC_SAYI_SMALL:
    MOV AX, OFFSET STR_ERR_SMALL
    CALL PRINT_STRING
    JMP KAC_SAYI_START
INVALID_KAC_SAYI_LARGE:
    MOV AX, OFFSET STR_ERR_LARGE
    CALL PRINT_STRING
    JMP KAC_SAYI_START
KAC_SAYI_END:
    MOV SAYILAR_LEN, AX    ; [1, 10] arası değeri tut

    POP AX
    RET
READ_SAYILAR_LEN ENDP



; -------------------------------------------------------- ;
; AX'de adresi verilen 0 terminated string'i yazdır, "PUTS"
PRINT_STRING PROC NEAR
        PUSH BX 
        MOV BX, AX
        MOV AL, BYTE PTR [BX]
    PUT_LOOP:   
        CMP AL, 0        
        JE  PUT_FIN
        CALL PRINT_CHAR
        INC BX
        MOV AL, BYTE PTR [BX]
        JMP PUT_LOOP
    PUT_FIN:
        POP BX
        RET 
PRINT_STRING ENDP


; -------------------------------------------------------- ;
; AL'deki karakteri print et, "PUTC"
PRINT_CHAR PROC NEAR
    PUSH AX
    PUSH DX
    MOV DL, AL
    MOV AH, 2
    INT 21H
    POP DX
    POP AX
    RET 
PRINT_CHAR ENDP 


; -------------------------------------------------------- ;
; AX'te bulunan sayıyı decimal olarak print eder, "PUTN"
PRINT_NUMBER PROC NEAR
        PUSH CX
        PUSH DX     
        XOR DX, DX        
        PUSH DX           
        MOV CX, 10        
        CMP AX, 0
        JGE CALC_DIGITS 
        NEG AX            
        PUSH AX           
        MOV AL, '-'       
        CALL PRINT_CHAR
        POP AX            
        
    CALC_DIGITS:
        DIV CX            
        ADD DX, '0'       
        PUSH DX           
        XOR DX,DX         
        CMP AX, 0         
        JNE CALC_DIGITS   
        
    DISP_LOOP:
        POP AX            
        CMP AX, 0         
        JE END_DISP_LOOP 
        CALL PRINT_CHAR         
        JMP DISP_LOOP     
        
    END_DISP_LOOP:
        POP DX 
        POP CX
        RET
PRINT_NUMBER ENDP 


; -------------------------------------------------------- ;
; Klavyeden karakter girdisi al ve AL'ye koy, "GETC"
SCAN_CHAR PROC NEAR
    MOV AH, 1h
    INT 21H
    RET 
SCAN_CHAR ENDP 


; -------------------------------------------------------- ;
; Klavyeden basılan sayıyı AX'e atar, "GETN"
SCAN_NUMBER PROC NEAR
        PUSH BX
        PUSH CX
        PUSH DX
    SCAN_NUMBER_START:
        MOV DX, 1        
        XOR BX, BX       
        XOR CX, CX       
    NEW:
        CALL SCAN_CHAR   
        CMP AL, CR 
        JE FIN_READ      
        CMP  AL, '-'     
        JNE  CTRL_NUM    
    NEGATIVE:
        MOV DX, -1       
        JMP NEW          
    CTRL_NUM:
        CMP AL, '0'      
        JB error 
        CMP AL, '9'
        JA error         
        SUB AL,'0'       
        MOV BL, AL       
        MOV AX, 10       
        PUSH DX          
        MUL CX           
        POP DX           
        MOV CX, AX       
        ADD CX, BX       
        JMP NEW          
    ERROR:
        MOV AX, OFFSET STR_ERR_SCAN_NUMBER 
        CALL PRINT_STRING           
        JMP SCAN_NUMBER_START  
    FIN_READ:
        MOV AX, CX             
        CMP DX, 1              
        JE FIN_SCAN_NUMBER
        NEG AX                 
    FIN_SCAN_NUMBER:
        POP DX
        POP CX
        POP BX
        RET 
SCAN_NUMBER ENDP 


; -------------------------------------------------------- ;


SEG_CODE ENDS
    END MAIN_PROC
