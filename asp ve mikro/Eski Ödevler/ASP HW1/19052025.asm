

STSG SEGMENT PARA STACK 'STSGM'
	DW 32 DUP (?)
STSG ENDS



DTSG	SEGMENT PARA 'DATA'

; string'ler
CR	EQU 13
LF	EQU 10
NAME_INFO             DB CR, LF, 'Oguzhan Topaloglu 19052025', 0
CHOOSE_STR            DB CR, LF, 'Yapmak istediginizi seciniz:', 0
OPTION1               DB CR, LF, '1 N tane terim girerek listeyi olusturma', 0
OPTION2               DB CR, LF, '2 Listeyi yazdir', 0
OPTION3               DB CR, LF, '3 Listeye terim ekle', 0
OPTION4               DB CR, LF, '4 Programi sonlandir', CR, LF, 0
SELECTED              DB CR, LF, 'Secimiz: ', 0
LINE_BREAK            DB CR, LF, ' ', 0
TO_ADD_STR            DB CR, LF, 'Eklemek istediginiz terim: ', 0
MENU_ERR	          DB CR, LF, CR, LF, '1,2,3,4 sayilarindan birisini secmelisiniz, yeniden secim yapiniz: ', 0
DATAS_STR             DB CR, LF, 'Veriler: ', 0
LINKS_STR             DB CR, LF, 'Linkler: ', 0
INPUT_STR             DB CR, LF, 'Eklenecek deger: ', 0
EMPTY_STR             DB CR, LF, 'Diziniz bos, lutfen deger giriniz...', 0
AMOUNT_STR            DB CR, LF, 'Kac terim gireceksiniz: ', 0
VALUES_STR            DB CR, LF, 'Diziniz: ', 0
ARROW_STR             DB ' -> ', 0
COMMA_STR             DB ' , ', 0
ALREADY_DEFINED_STR   DB CR, LF, 'Diziniz onceden tanimlandigindan ucuncu menuyu kullanmalisiniz...', 0

; linkedlist degerleri
ARR DW 50 DUP(?)
LINKS DW 50 DUP(?)
N DW 0

MIN_INDEX DW ?
MAX_INDEX DW ?

; bu degerler fonksiyon icinde yazmaclar yuzunden kafam karismasin diye tanimlandi, otomatik doldurulacaklar ve hicbir onemleri yok
MIN_VAL DW ?
MAX_VAL DW ?

DTSG 	ENDS 



CDSG SEGMENT PARA 'CDSGM'
	ASSUME CS: CDSG, DS: DTSG, SS: STSG
	

	; --------------- MAIN PROSEDUR ---------------
	MAIN PROC FAR 
 
 		; exe islemleri
		PUSH DS
		XOR AX, AX
		PUSH AX

		MOV AX, DTSG
		MOV DS, AX
		
		; ------------------- ASIL KOD -------------------

INF_LOOP:
		
		; menuyu yazdirma ve girdi isteme
		MOV AX, OFFSET NAME_INFO
		CALL PUT_STR
		MOV AX, OFFSET CHOOSE_STR
		CALL PUT_STR
		MOV AX, OFFSET OPTION1
		CALL PUT_STR
		MOV AX, OFFSET OPTION2
		CALL PUT_STR
		MOV AX, OFFSET OPTION3
		CALL PUT_STR
		MOV AX, OFFSET OPTION4
		CALL PUT_STR
		MOV AX, OFFSET SELECTED
		CALL PUT_STR
		CALL GETN ; girdiyi isteme

		; verilen girdiye gore belirli prosedurleri calistirma

		CMP AX, 1  ; girdi = 1 ise CREATE_LIST calistir
		JNZ CONTINUE1 ; 1 degilse devam et
		CALL CREATE_LIST
CONTINUE1:
		CMP AX, 2  ; girdi = 2 ise PRINT_LIST calistir
		JNZ CONTINUE2 ; 2 degilse devam et
		CALL PRINT_LIST
CONTINUE2:
		CMP AX, 3  ; girdi = 3 ise ADD_TERM_TO_LIST calistir
		JNZ CONTINUE3 ; 3 degilse devam et
		CALL ADD_TERM_TO_LIST
CONTINUE3:
		CMP AX, 4  ; girdi = 4 ise programi kapat
		JNZ INF_LOOP ; 4 degilse basa don artik
		JMP END_PROGRAM

		JMP INF_LOOP
END_PROGRAM:

		; ------------------- ASIL KOD -------------------

		RETF
	MAIN ENDP



	; --------------- CREATE_LIST PROSEDUR ---------------
	; ADD_TERM_TO_LIST prosedurunu N kez calistirarak N boyutlu linkedlist olusturur
	; PUSH ve POP komutlari kullanmadigimdan AX, CX degisir
	;
	CREATE_LIST PROC NEAR

		CMP N, 0
		JZ CONT_LB
		MOV AX, OFFSET ALREADY_DEFINED_STR
		CALL PUT_STR
		
		MOV AX, OFFSET LINE_BREAK
		CALL PUT_STR
		RET
	CONT_LB:
		MOV AX, OFFSET AMOUNT_STR
		CALL PUT_STR

		CALL GETN
		MOV CX, AX
		CMP CX, 0
		JLE LOOP_END  ; CX <= 0 ise menu kapanmali
		
		MOV AX, OFFSET LINE_BREAK
		CALL PUT_STR


	LOOP_START:

		CALL ADD_TERM_TO_LIST

		DEC CX
		CMP CX, 0
		JZ LOOP_END
		JMP LOOP_START
	LOOP_END:

		RET
	CREATE_LIST ENDP





	; --------------- ADD_TERM_TO_LIST PROSEDUR ---------------
	; linkedlist'e bir terim eklemeye yarar
	; if() else if() else if() else  seklinde bir yapi ile kontrol yapip terimi ekler
	; PUSH ve POP komutlari kullanmadigimdan yazmaclar degisir
	; tum kodda adam akilli prosedur yalnizca bu vardi diye PUSH POP yapmadim
	; zaten baska prosedur olmayacak yazmaclari sikca kullanan
	;
	;
	ADD_TERM_TO_LIST PROC NEAR
		PUSH CX

		MOV AX, OFFSET INPUT_STR
		CALL PUT_STR
		CALL GETN ; AX girilen değer

		CMP N, 0
		JNZ CONT1
		; N = 0 yani hiç değer yok ilk değer girilecek
		MOV ARR[0], AX
		MOV LINKS[0], -1
		MOV MIN_INDEX, 0
		MOV MAX_INDEX, 0
		INC N
		JMP END_FUNC
	CONT1:

		; if sonu
		; bu if'in gerisinde kullanma amacli min ve max degerlerini cikartma
		; min deger cikartma
		MOV BX, MIN_INDEX
		SHL BX, 1
		MOV BX, ARR[BX]
		MOV MIN_VAL, BX
		; max deger cikartma
		MOV BX, MAX_INDEX
		SHL BX, 1
		MOV BX, ARR[BX]
		MOV MAX_VAL, BX
		; if'e devam

		CMP AX, MIN_VAL   ; AX <= min_val olmali yani AX > min_val ise ziplamaliyim
		JNLE CONT2
		; min deger girildi
		MOV BX, N
		SHL BX, 1                    ; offset'i elde et

		MOV ARR[BX], AX              ; yeni deger en sona eklenecek

		MOV DX, MIN_INDEX
		MOV LINKS[BX], DX     ; eski min index'i son eklenenin index'i olacak

		MOV DX, N
		MOV MIN_INDEX, DX             ; son eklenen terim yeni minimum index'imiz

		INC N
		JMP END_FUNC
	CONT2:

		CMP AX, MAX_VAL   ; AX >= max_val olmali yani AX < max_val ise ziplamaliyim
		JNGE CONT3
		; max deger girildi
		MOV BX, N
		SHL BX, 1    ; offset

		MOV ARR[BX], AX     ; en son eklenen terimi ekle
		MOV LINKS[BX], -1   ; en son eklenen max diye -1 linkli olacak

		MOV BX, MAX_INDEX
		SHL BX, 1
		MOV DX, N
		MOV LINKS[BX], DX    ; eski max degeri yeni eklenen degere (yeni max'a) bakacak

		MOV MAX_INDEX, DX
		INC N

		JMP END_FUNC
	CONT3:
		; buraya gelirse max da degil min de degil ilk terim de degildir
		; yani kesinlikle iki deger arasindadir

		; burada x ve y arasina z giriyor diyelim, birbiriyle ayni degerler olabileceginden
		; tum terimleri linkleri kullanarak gezmeliyim ve en son <= val ile uyan terimi x olarak almaliyim
		; y dedigim de x'in link'i ile kolayca bulunabilir
		; islem sirasinde x_link (y_index) ve x_index gerekli

		; x_link belirleme
		MOV SI, MIN_INDEX
		SHL SI, 1

		MOV CX, N
	MY_LABEL2:
		MOV BX, ARR[SI]

		CMP BX, AX
		JNG STILL_NO  ; BX > AX ise kirmali
		JMP END_MY_LOOP
	STILL_NO:
		MOV SI, LINKS[SI]
		SHL SI, 1          ; linkler dogrudan indeks o yuzden offset elde etmek icin x2 yap

		LOOP MY_LABEL2
	END_MY_LOOP:
		SHR SI, 1
	
		; burada SI dedigimiz sey x_link

		; simdi de SI (x_link) kullanarak x_index bulacagiz
		; x_index belirleme
		MOV BP, 0

		MOV CX, 0
	START_LOOP:
		MOV BX, LINKS[BP]

		CMP BX, SI
		JZ FOUND

		ADD BP, 2

		INC CX
		CMP CX, N
		JNZ START_LOOP
	FOUND:

		; burada CX dedigimiz sey x_index, AX dedigimiz sey yeni_deger, SI dedigimiz sey x_link
		; simdi gerekli seyleri buldugumuza gore istedigimiz islemleri yapabiliriz
		; yeni eklenen terimi ekleyelim
		MOV BX, N
		SHL BX, 1 ; offset
		MOV ARR[BX], AX
		; yeni eklenen terimin linkini (z_link) gidip y_index = x_link'e esitleyelim
		MOV LINKS[BX], SI
		; x teriminin indeksini guncelleyelim
		MOV BX, CX
		SHL BX, 1
		MOV BP, N
		MOV LINKS[BX], BP
		; terim sayisini arttiralim
		INC N
		

	END_FUNC:
		MOV AX, OFFSET LINE_BREAK
		CALL PUT_STR

		
		POP CX
		RET
	ADD_TERM_TO_LIST ENDP





	; --------------- PRINT_LIST PROSEDUR ---------------
	; once diziyi dumduz bir sekilde yazdirir, sonra linkleri dumduz bir sekilde yazdirir
	; ondan sonra da linkleri kullanarak diziyi kucukten buyuge siralanmis yani sorted bir sekilde yazdirir
	; PUSH POP yine kullanilmadi cunku gerek yok yalnizca ADD_TERM_TO_LIST proseduru var yazmaclari sik sik kullanan
	;
	PRINT_LIST PROC NEAR

		; eger dizi tanimli degilse yazdirmadan don
		CMP N, 0
		JNZ NOT_EMPTY
		MOV AX, OFFSET EMPTY_STR
		CALL PUT_STR
		MOV AX, OFFSET LINE_BREAK
		CALL PUT_STR
		RET
	NOT_EMPTY:


		; Veriler: kisminin yazdirilmasi
		MOV AX, OFFSET DATAS_STR
		CALL PUT_STR

		MOV CX, N
		MOV BX, 0
	LB11:
		MOV AX, ARR[BX]
		CALL PUTN

		CMP CX, 1
		JZ NO_PRINT
		MOV AX, OFFSET COMMA_STR
		CALL PUT_STR
	NO_PRINT:
		
		ADD BX, 2
		DEC CX
		CMP CX, 0
		JZ END_LB1
		JMP LB11
	END_LB1:


		; Linkler: kisminin yazdirilmasi
		MOV AX, OFFSET LINKS_STR
		CALL PUT_STR

		MOV CX, N
		MOV BX, 0
	LB22:
		MOV AX, LINKS[BX]
		CALL PUTN

		CMP CX, 1
		JZ NO_PRINT2
		MOV AX, OFFSET COMMA_STR
		CALL PUT_STR
	NO_PRINT2:
		
		ADD BX, 2
		DEC CX
		CMP CX, 0
		JZ END_LB2
		JMP LB22
	END_LB2:

		
		; Listenin okunup linklere gore sorted sekilde yazdirilmasi
		MOV AX, OFFSET VALUES_STR
		CALL PUT_STR

		MOV SI, MIN_INDEX
		SHL SI, 1           ; min_index dedigimiz dogrudan indeks o yuzden offset elde etmek icin x2 yap

		MOV CX, N
	MY_LABEL:
		MOV AX, ARR[SI]
		CALL PUTN

		; en son terimden sonra bitirmeden once ok yazilmamali
		CMP CX, 1      
		JZ END_PROC

		MOV AX, OFFSET ARROW_STR
		CALL PUT_STR

		MOV SI, LINKS[SI]   
		SHL SI, 1          ; linkler dogrudan indeks o yuzden offset elde etmek icin x2 yap
		
	END_PROC:
		LOOP MY_LABEL

		MOV AX, OFFSET LINE_BREAK
		CALL PUT_STR

		RET

	PRINT_LIST ENDP







	; --------------- GETC PROSEDUR ---------------
	; ders kitabinin github sayfasindan alintidir, derste sormustum ve kopyala yapistir yapabilirsiniz demistiniz
	; klavyeden basilan karakteri AL'ye atar, yalnizca AL etkilenir
	;
	GETC PROC NEAR
		MOV AH, 1H
		INT 21H
		RET
	GETC ENDP




	; --------------- PUTC PROSEDUR ---------------
	; ders kitabinin github sayfasindan alintidir, derste sormustum ve kopyala yapistir yapabilirsiniz demistiniz
	; AL yazmacindaki degeri ekranda gosterir
	; DL ve AH degisiyor diye AX ve DX yazmaclarini saklar
	;
	PUTC PROC NEAR
		PUSH AX
		PUSH DX
		MOV DL,AL
		MOV AH,2
		INT 21H
		POP DX
		POP AX
		RET
	PUTC ENDP



	; --------------- GETN PROSEDUR ---------------
	; ders kitabinin github sayfasindan alintidir, derste sormustum ve kopyala yapistir yapabilirsiniz demistiniz
	; klavyeden basilan sayiyi okur, sonucu AX uzerine koyar, BX,CX,DX yazmaclari saklanir yalnizca AX etkilenir
	; DX: sayinin isaretli olup olmadigini belirler, 1 (+), -1 (-) demek
	; BL: hane bilgisini tutar
	; CX: okunan sayinin islenmesi sirasinda ara degerleri tutar
	; AL: klavyeden okunan karakteri ASCII olarak tutar
	; AX: donus degeri durumundadir
	;
	GETN PROC NEAR
		PUSH BX
		PUSH CX
		PUSH DX
		
	GETN_START:
		MOV DX,1
		XOR BX,BX
		XOR CX,CX
	NEW:
		CALL GETC
		CMP AL,CR
		JE FIN_READ
		CMP AL, '-'
		JNE CTRL_NUM
	NEGATIVE:
		MOV DX, -1
		JMP NEW
	CTRL_NUM:
		CMP AL, '0'
		JB ERROR
		CMP AL, '9'
		JA ERROR
		SUB AL,'0'
		MOV BL,AL
		MOV AX,10
		PUSH DX 
		MUL CX
		POP DX
		MOV CX,AX
		ADD CX,BX
		JMP NEW
	ERROR:
		MOV AX, OFFSET MENU_ERR
		CALL PUT_STR
		JMP GETN_START
	FIN_READ:
		MOV AX,CX
		CMP DX,1
		JE FIN_GETN
		NEG AX
	FIN_GETN:
		POP DX
		POP CX
		POP BX
		RET
	GETN ENDP




	; --------------- PUTN PROSEDUR ---------------
	; ders kitabinin github sayfasindan alintidir, derste sormustum ve kopyala yapistir yapabilirsiniz demistiniz
	; AX'de bulunan sayiyi onluk tabanda hane hane yazdirir, CX ve DX saklanir
	; CX: haneleri 10'a bolmeye yarar, CX = 10 olacak
	; DX: 32 bolmede isleme dahil olacak, sonucu etkilemesin diye 0 olmali
	;
	PUTN PROC NEAR
		PUSH CX
		PUSH DX
		XOR DX, DX
		PUSH DX
		MOV CX, 10
		CMP AX,0
		JGE CALC_DIGITS
		NEG AX
		PUSH AX
		MOV AL, '-'
		CALL PUTC
		POP AX
	CALC_DIGITS:
		DIV CX
		ADD DX,'0'
		PUSH DX
		XOR DX,DX
		CMP AX,0
		JNE CALC_DIGITS
	DISP_LOOP:
		POP AX
		CMP AX,0
		JE END_DISP_LOOP
		CALL PUTC
		JMP DISP_LOOP
	END_DISP_LOOP:
		POP DX
		POP CX
		RET
	PUTN ENDP




	; --------------- PUT_STR PROSEDUR ---------------
	; ders kitabinin github sayfasindan alintidir, derste sormustum ve kopyala yapistir yapabilirsiniz demistiniz
	; AX'de adresi verilen sonunda 0 olan dizgeyi karakter karakter yazdirir
	; BX: dizgiye indis olarak kullanilir, onceki degeri saklanmalidir
	;
	PUT_STR PROC NEAR
		PUSH BX
		MOV BX, AX
		MOV AL, BYTE PTR [BX]
	PUT_LOOP:
		CMP AL,0
		JE PUT_FIN
		CALL PUTC
		INC BX
		MOV AL, BYTE PTR [BX]
		JMP PUT_LOOP
	PUT_FIN:
		POP BX
		RET
	PUT_STR ENDP



CDSG ENDS

	END MAIN

