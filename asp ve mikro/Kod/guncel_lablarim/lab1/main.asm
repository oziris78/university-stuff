

;;;  --------------------------------------------------------  ;;;
;;;  |  BLM3061 - Mikroişlemci Sistemleri ve Assembly Dili  |  ;;;
;;;  |     Lab #1, Oğuzhan Topaloğlu, Ç19052025, Grup2      |  ;;;
;;;  --------------------------------------------------------  ;;;

;;;  UNLISTED YOUTUBE VIDEO LINKI:
;;;      https://youtu.be/4K2Hu6Vb63U


SEG_STACK SEGMENT PARA STACK 'yigin'
    DW 20 DUP (?)
SEG_STACK ENDS


SEG_DATA SEGMENT PARA 'veri'
    ; 77*0.4 + 56*0.6  =>  64.4  =>  64  =>  0x40
    ; 85*0.4 + 63*0.6  =>  71.8  =>  72  =>  0x48
    ; 64*0.4 + 86*0.6  =>  77.2  =>  77  =>  0x4D
    ; 96*0.4 + 74*0.6  =>  82.8  =>  83  =>  0x53
    OBPLER DB ?, ?, ?, ?
    
    VIZELER DB 77, 85, 64, 96    ; 4D 55 40 60
    FINALLER DB 56, 63, 86, 74   ; 38 3F 56 4A
    N DW 4                       ; terim sayısı
SEG_DATA ENDS


SEG_CODE SEGMENT PARA 'kod'
    ASSUME CS: SEG_CODE, DS: SEG_DATA, SS: SEG_STACK
    
    MAIN_PROC PROC FAR
        PUSH DS
        XOR AX, AX
        PUSH AX
        MOV AX, SEG_DATA
        MOV DS, AX
        
        ; ----------------------  KOD BAŞLANGICI  ---------------------- ;

        ; CX = N, SI -> vizeler, DI -> finaller, BX -> obpler
        LEA BX, OBPLER
        LEA SI, VIZELER
        LEA DI, FINALLER
        MOV CX, N

        ; OBP = round(vize * 0.4 + final * 0.6) = round( (40*vize + 60*final)/100 )
        ; round için kullandığım trick:
        ;     AX = 274? (27.4?)  =>  AX = 279? (27.9?)  =>  27
        ;     AX = 275? (27.5?)  =>  AX = 280? (28.0?)  =>  28
        ;     AX = 276? (27.6?)  =>  AX = 281? (28.1?)  =>  28
    NEXT_ITEM:
        ; 40 * vize hesaplama
        MOV AH, 0         ; 16-bit çarpma için
        MOV AL, [SI]      ; AL = bu iterasyondaki vize
        MOV DL, 40
        MUL DL            ; AX = vize * 40
        MOV DX, AX        ; DX = vize * 40 sonucu sakla

        ; 60 * final hesaplama
        MOV AH, 0         ; 16-bit çarpma için
        MOV AL, [DI]      ; AL = bu iterasyondaki final
        PUSH DX
            MOV DL, 60
            MUL DL        ; AX = final * 60
        POP DX
        ADD AX, DX        ; AX = final * 60 + vize * 40

        ; 100'e bölme ve yuvarlama
        ADD AX, 50        ; yuvarlama yapmak için bir trick
        
        PUSH CX
            MOV CL, 100
            DIV CL        ; AH = kalan, AL = OBP (sonuç)
        POP CX

        MOV [BX], AL      ; OBP değerini OBPLER dizisine kaydet

        ; Dizileri ilerlet
        INC SI            ; VIZELER dizisinde indeks arttır
        INC DI            ; FINALLER dizisinde indeks arttır
        INC BX            ; OBPLER dizisinde indeks arttır
        LOOP NEXT_ITEM    ; CX yani N sıfırlanınca loop çalışmaz

        ; OBPLER dizisini bubble sort ile sıralama
        XOR SI, SI                  ; SI dış döngü için indeks
        MOV CX, N
        DEC CX                      ; CX = N-1, son terimin sağında yok diye -1
    OUTER_LOOP: 
        PUSH CX
        MOV CX, N
        SUB CX, SI                  ; CX = CX - SI, dış döngü sayısını azalt
        DEC CX
    
        XOR DI, DI                  ; DI iç döngü için indeks
    INNER_LOOP: 
        MOV AL, OBPLER[DI]
        CMP AL, OBPLER[DI + 1]
        JG DONT_SWAP                ; soldaki > sağdaki ise swapping yapılmayacak
        XCHG AL, OBPLER[DI + 1]
        MOV OBPLER[DI], AL
    DONT_SWAP:
        INC DI                      ; iç döngü indeksini arttır
        LOOP INNER_LOOP
        INC SI                      ; dış döngü indeksini arttır
        POP CX                      ; iç döngü için saklanan değeri geri yükle
        LOOP OUTER_LOOP


        ; ----------------------  KOD BİTİŞİ  ---------------------- ;
        
        RETF
    MAIN_PROC ENDP

SEG_CODE ENDS
    END MAIN_PROC

