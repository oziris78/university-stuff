/*
	-- Yapisal Programlama PROJESI -- 
	Ad Soyad:  Oguzhan Topaloglu
	Numara:    C19052025 (CAP ogrencisiyim)
	Grup:      Ahmet Elbir
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define MAX_LEN 1024
#define RAFTA_STR "Rafta"



////////////////////////////////////////////
////////////  STRUCT TANIMLARI  ////////////
////////////////////////////////////////////


/* NOTLAR
	- bazi csv dosyalarinda turkce karakter sikintisi oldugu icin o 
		karakterleri soru drive'inda denildigi gibi degistirdim
		
	- soru drive'inda struct tanimlarinda typedef kullanabilirsiniz 
		denildiginden tanimlarda typedef kullandim.
		
	- soru drive'inda da denildigi gibi bazi struct'lardaki field'larin 
		isimlerini camelCase'e uygun bir sekilde guncelledim ve duzelttim,
		ornegin "YazarID" yerine "yazarID" yazdim.
*/

// ogrID, \0 a yer kalmadigi icin 8 degil 9 olmali, soru drive'inda denildi
typedef struct Ogrenci {
	char ogrID[9];  
	char ad[30];
	char soyad[30];
	int puan;
	struct Ogrenci* next;
	struct Ogrenci* prev;
} Ogrenci;


typedef struct Yazar {
	int yazarID;
	char yazarAd[30];
	char yazarSoyad[30];
	struct Yazar* next;
} Yazar;


typedef struct KitapOrnek {
	char etiketNo[20];
	char durum[8];
	struct KitapOrnek* next;
} KitapOrnek;


typedef struct Kitap {
	char kitapAdi[30];
	char ISBN[13];
	int adet;
	struct Kitap* next;
	struct KitapOrnek* head;
} Kitap;


typedef struct KitapYazar {
	char ISBN[13];
	int yazarID;
} KitapYazar;


// soru drive'inda unsigned int yapabiliriz denildi
typedef struct Tarih {
	unsigned int gun:5; 
	unsigned int ay:4; 
	unsigned int yil:12;
} Tarih;


// ogrID, \0 a yer kalmadigi icin 8 degil 9 olmali, soru drive'inda denildi
// islemTipi'ni unsigned yapabilir miyim diye Ahmet hocama mail attim kendisi olur dedi
typedef struct KitapOdunc {
	char etiketNo[20];
	char ogrID[9];
	unsigned int islemTipi:1;
	struct Tarih islemTarihi;
} KitapOdunc;



///////////////////////////////////////////////
////////////  FONKSIYON TANIMLARI  ////////////
///////////////////////////////////////////////


/* MENU FONKSIYONLARI */
void printLateTurnIn(Kitap**, char**, int*);
void listWaitedStudents(Ogrenci**, Kitap**, KitapOdunc**, int*);
void updateStudent(Ogrenci**, const char*);
void deleteStudent(Ogrenci**, const char*);
void addStudent(Ogrenci**, const char*);
void listAllStudents(Ogrenci**);
void listPunishedStudents(Ogrenci**);
void searchStudent(Ogrenci**, KitapOdunc**, int*);
void addBook(Kitap**, const char*);
void deleteBook(Kitap**, const char*, KitapYazar**, int*, const char*);
void updateBook(Kitap**, const char*);

void updateBooksAuthor(Kitap**, Yazar**, KitapYazar**, int*, const char*);
void updateGivenAuthor(Yazar**, char*, char*, const char*);
void updateAuthor(Yazar**, const char*);
void deleteAuthor(Yazar**, const char*, KitapYazar**, const char*, int*);
void deleteAuthorFromKitapYazar(KitapYazar**, Yazar*, const char*, int*);
void addAuthor(Yazar**, const char*);
void showAuthorInfo(Yazar**, KitapYazar**, int*, Kitap**);
void pairBookAndAuthor(KitapYazar**, int*, Kitap**, Yazar**, const char*);

void listAvailableBookSamples(Kitap**);
void searchBooks(Kitap** kitaplar);
void borrowBook(KitapOdunc**, int*, const char*, Ogrenci**, Kitap**);
void turnInBook(KitapOdunc**, int*, const char*, Ogrenci**, Kitap**, char**, int*);


/* PRINTING/STRING FONKSIYONLARI */
void printStudent(Ogrenci*);
void printAuthor(Yazar*);
void printAllBookInfo(Kitap**, char*);
void listAllAuthors(Yazar**);
void listAllStudents(Ogrenci**);
char* studentToString(Ogrenci*);
char* authorToString(Yazar*);


/* DOSYA FONKSIYONLARI */
void writeKitapOdunc(const char*, KitapOdunc**, int*);
void writeBooks(Kitap**, const char*);
void writeKitapYazar(KitapYazar**, const char*, int*);

Ogrenci* readStudents(const char*);
Yazar* readAuthors(const char*);
Kitap* readBooks(const char*);
KitapOdunc* readBorrowed(const char*, int*);
KitapYazar* readBookAuthor(Kitap**, Yazar**, const char*, int*);

void sortBooks(Kitap**);
void processBorrowed(Kitap**, Ogrenci**, KitapOdunc**, int*, char**, int*);


/* YARDIMCI (HELPER) FONKSIYONLAR */
void saveLateTurnIn(char**, int*, char*);
void filterFile(const char*, const int, int(*f)(char*,char*), char*, char*);
FILE* openFile(const char*, const char*);
int getNumberInRange(int, int);
int strNotEqual(char*, char*);
int authorEqualsTo(Yazar*, char*, char*);
void findAuthorByFullname(Yazar**, Yazar**, Yazar**, char*, char*);
Yazar* findAuthorByID(Yazar**, int);
Kitap* findBook(Kitap** head, int(*cmpFunc)(Kitap*, char*), char* cmpStr);
Ogrenci* findStudent(Ogrenci**, int(*cmpStudent)(Ogrenci*, char*), char*);
int isDateValid(int, int, int);
int cmpBookISBN(Kitap*, char*);
int cmpBookName(Kitap*, char*);
int cmpStudentID(Ogrenci*, char*);
KitapOrnek* getBookSample(Kitap**, char*, int);
int getInformation(Ogrenci**, Ogrenci**, int*, int*, int*);



///////////////////////////////////////////////
//////////////  MAIN FONKSIYONU  //////////////
///////////////////////////////////////////////




int main() {
	srand(time(NULL));
	printf("\nOguzhan Topaloglu, C19052025, Yapisal Programlama Projesi\n\n");
	
	const char* pathKitaplar = "Kitaplar.csv";
	const char* pathKitapYazar = "KitapYazar.csv";
	const char* pathOgrenciler = "Ogrenciler.csv";
	const char* pathYazarlar = "Yazarlar.csv";
	const char* pathKitapOdunc = "KitapOdunc.csv";
	
	Ogrenci* ogrenciler = readStudents(pathOgrenciler);
	Yazar* yazarlar = readAuthors(pathYazarlar);
	Kitap* kitaplar = readBooks(pathKitaplar);
	
	// 1D ARRAY
	int lateTurnInCount;
	char* lateTurnInISBN = NULL; 
	
	// 1D ARRAY
	int bookAuthorCount;
	KitapYazar* kitapYazar = readBookAuthor(&kitaplar, &yazarlar, pathKitapYazar, &bookAuthorCount);
	
	// 1D ARRAY
	int borrowedCount;
	KitapOdunc* oduncler = readBorrowed(pathKitapOdunc, &borrowedCount);
	processBorrowed(&kitaplar, &ogrenciler, &oduncler, &borrowedCount, &lateTurnInISBN, &lateTurnInCount);
	
	int menuOption, menuSubOption, running = 1;
	while(running) {
		printf("\n-------------------- MENU --------------------\n");
		printf("01- Ogrenci ekle/sil/guncelle\n");
		printf("02- Ogrenci bilgisi goruntuleme\n");
		printf("03- Kitap teslim etmemis ogrencileri listele\n");
		printf("04- Cezali ogrencileri listele\n");
		printf("05- Tum ogrencileri listele\n");
		printf("06- Kitap odunc al/teslim et\n");
		
		printf("07- Kitap ekle/sil/guncelle\n");
		printf("08- Kitap bilgisi goruntuleme\n");
		printf("09- Raftaki kitaplari listele\n");
		printf("10- Zamaninda teslim edilmeyen kitaplari listele\n");
		printf("11- Kitap-yazar eslestir\n");
		printf("12- Kitabin yazarini guncelle\n");
		
		printf("13- Yazar ekle/sil/guncelle\n");
		printf("14- Yazar bilgisi goruntuleme\n");
		printf("15- Tum yazarlari listele\n"); // ben ekledim
		
		printf("16- Programi kapatma\n\n");
		
		printf("Lutfen bir secim yapiniz: ");
		scanf("%d", &menuOption);
		
		switch(menuOption){
			case 1:
				printf("\n--- Alt menu ---\n");
				printf("1- Ogrenci ekle\n");
				printf("2- Ogrenci sil\n");
				printf("3- Ogrenci guncelle\n");
				printf("4- Alt menuden cikis\n\n");

				menuSubOption = getNumberInRange(1, 4);
				if(menuSubOption == 1) addStudent(&ogrenciler, pathOgrenciler);
				if(menuSubOption == 2) deleteStudent(&ogrenciler, pathOgrenciler);
				if(menuSubOption == 3) updateStudent(&ogrenciler, pathOgrenciler);
				break;
			case 2:
				searchStudent(&ogrenciler, &oduncler, &borrowedCount);
				break;
			case 3:
				//  Kitap teslim etmemis ogrencileri listele
				listWaitedStudents(&ogrenciler, &kitaplar, &oduncler, &borrowedCount); 
				break;
			case 4:
				listPunishedStudents(&ogrenciler);
				break;
			case 5:
				listAllStudents(&ogrenciler);
				break;
			case 6:
				printf("\n--- Alt menu ---\n");
				printf("1- Kitap odunc al\n");
				printf("2- Kitap teslim et\n");
				printf("3- Alt menuden cikis\n");
				
				menuSubOption = getNumberInRange(1, 3);
				if(menuSubOption == 1) borrowBook(&oduncler, &borrowedCount, pathKitapOdunc, &ogrenciler, &kitaplar);
				if(menuSubOption == 2) turnInBook(&oduncler, &borrowedCount, pathKitapOdunc, &ogrenciler, 
					&kitaplar, &lateTurnInISBN, &lateTurnInCount);
				break;
			case 7:
				printf("\n--- Alt menu ---\n");
				printf("1- Kitap ekle\n");
				printf("2- Kitap sil\n");
				printf("3- Kitap guncelle\n");
				printf("4- Alt menuden cikis\n\n");

				menuSubOption = getNumberInRange(1, 4);
				if(menuSubOption == 1) addBook(&kitaplar, pathKitaplar);
				if(menuSubOption == 2) deleteBook(&kitaplar, pathKitaplar, &kitapYazar, &bookAuthorCount, pathKitapYazar);
				if(menuSubOption == 3) updateBook(&kitaplar, pathKitaplar);
				break;
			case 8:
				searchBooks(&kitaplar);
				break;
			case 9:
				listAvailableBookSamples(&kitaplar);
				break;
			case 10:
				// Zamaninda teslim edilmeyen kitaplari listele
				printLateTurnIn(&kitaplar, &lateTurnInISBN, &lateTurnInCount);
				break;
			case 11:
				pairBookAndAuthor(&kitapYazar, &bookAuthorCount, &kitaplar, &yazarlar, pathKitapYazar);
				break;
			case 12:
				updateBooksAuthor(&kitaplar, &yazarlar, &kitapYazar, &bookAuthorCount, pathYazarlar);
				break;
			case 13:
				printf("\n--- Alt menu ---\n");
				printf("1- Yazar ekle\n");
				printf("2- Yazar sil\n");
				printf("3- Yazar guncelle\n");
				printf("4- Alt menuden cikis\n\n");

				menuSubOption = getNumberInRange(1, 4);
				if(menuSubOption == 1) addAuthor(&yazarlar, pathYazarlar);
				if(menuSubOption == 2) deleteAuthor(&yazarlar, pathYazarlar, &kitapYazar, pathKitapYazar, &bookAuthorCount);
				if(menuSubOption == 3) updateAuthor(&yazarlar, pathYazarlar);
				break;
			case 14:
				showAuthorInfo(&yazarlar, &kitapYazar, &bookAuthorCount, &kitaplar);
				break;
			case 15:
				listAllAuthors(&yazarlar);
				break;
			case 16:
				printf("Program kapatiliyor...\n");
				running = 0;
				break;
			default:
				printf("Gecersiz giris yaptiniz, lutfen [1, 15] arasi bir int giriniz.\n");
		}
	}
	
	// free memory
	while (ogrenciler != NULL) {
		Ogrenci* next = ogrenciler->next;
		free(ogrenciler);
		ogrenciler = next;
	}
	while (yazarlar != NULL) {
		Yazar* next = yazarlar->next;
		free(yazarlar);
		yazarlar = next;
	}
	while (kitaplar != NULL) {
		Kitap* next = kitaplar->next;
		KitapOrnek* ornekHead = kitaplar->head;
		while (ornekHead != NULL) {
			KitapOrnek* nn = ornekHead->next;
			free(ornekHead);
			ornekHead = nn;
		}
		free(kitaplar);
		kitaplar = next;
	}
	free(lateTurnInISBN);
	free(kitapYazar);
	free(oduncler);
	
	return 0;
}




//////////////////////////////////////////////////
//////////////  MENU FONKSIYONLARI  //////////////
//////////////////////////////////////////////////



void printLateTurnIn(Kitap** kitaplar, char** lateTurnInISBN, int* count) {
	int i;
	char* etiket;
	char* dupEtiket;
	char* p;
	if(*count <= 0){
		printf("Gec teslim edilen kitap yok\n");
		return;
	}
	
	for(i = 0; i < *count; i++){
		etiket = &(*lateTurnInISBN)[i];
		dupEtiket = strdup(etiket);
		p = strtok(dupEtiket, "_");
		Kitap* kitap = findBook(kitaplar, cmpBookISBN, p);
		printf("%s (%s) gec teslim edildi\n", kitap->kitapAdi, etiket);
		free(dupEtiket);
	}
}


void listWaitedStudents(Ogrenci** ogrenciler, Kitap** kitaplar, KitapOdunc** oduncler, int* bbCount) {
	printf("Teslimi beklenen kitaplar ve ogrenciler: \n");
	Kitap* temp = *kitaplar;
	int i, j;
	while(temp != NULL){
		KitapOrnek* ornekHead = temp->head;
		for(i = 0; i < temp->adet; i++){
			if(strcmp(ornekHead->durum, RAFTA_STR) != 0) { // teslim edilmesi bekleniyorsa
				for(j = 0; j < *bbCount; j++){
					KitapOdunc* od = &(*oduncler)[j];
					if(strcmp(od->etiketNo, ornekHead->etiketNo) == 0) {
						Ogrenci* student = findStudent(ogrenciler, cmpStudentID, od->ogrID);
						char* studStr = studentToString(student);
						if(od->islemTipi == 0)
							printf("\t%s %s ogrencisinin %s kitabini teslim etmesi bekleniyor\n", student->ad, student->soyad,
								od->etiketNo);
						free(studStr);
					}
				}
			}
			ornekHead = ornekHead->next;
		}
		temp = temp->next;
	}
}


void listPunishedStudents(Ogrenci** ogrenciler) {
	printf("\nCezali ogrenciler ve puanlari: \n");
	int found = 0;
	Ogrenci* ref = *ogrenciler;
	while(ref != NULL) {
		if(ref->puan < 100) {
			found = 1;
			int count = (100 - ref->puan) / 10;
			printf("\t%s %s, %d kez ceza almistir ve puani %d'dir.\n", ref->ad, ref->soyad, count, ref->puan);
		}
		ref = ref->next;
	}
	if(!found)
		printf("Cezali ogrenci bulunmamaktadir. :)\n");
}

void borrowBook(KitapOdunc** oduncler, int* bbCount, const char* bbPath, Ogrenci** ogrenciler, Kitap** kitaplar) {
	const int islemTuru = 0;
	Ogrenci* student;
	int gun, ay, yil;	
	if(!getInformation(ogrenciler, &student, &gun, &ay, &yil))
		return;
	
	printf("Lutfen odunc almak istediginiz kitabin ISBN'ini giriniz: ");
	char ISBN[20];
	scanf("%s", ISBN);
	
	// ISBN'li kitap var mi kontrolu
	Kitap* book = findBook(kitaplar, cmpBookISBN, ISBN);
	if(book == NULL) {
		printf("%s ISBN'li bir kitap bulunmamaktadir...\n", ISBN);
		return;
	}
	
	// bu kitabin rafta herhangi bir ornegi var mi kontrolu
	KitapOrnek* ilkOrnek = NULL;
	KitapOrnek* ref = book->head;
	while(ref != NULL && ilkOrnek == NULL){
		if(strcmp(ref->durum, RAFTA_STR) == 0){
			ilkOrnek = ref;
		}
		ref = ref->next;
	}
	if(ilkOrnek == NULL) {
		printf("%s ISBN'li kitabin rafta ornegi bulunmamaktadir...\n", ISBN);
		return;
	}
	
	// ogrencinin puani negatifse odunc alamaz
	if(student->puan < 0) {
		printf("Puaniniz %d oldugundan bir kitabi odunc alamazsiniz!\n", student->puan);
		return;
	}
	
	// odunc al
	strcpy(ilkOrnek->durum, student->ogrID);
	
	// en sona ekle ve dosyayi yeniden yaz
	*bbCount = *bbCount + 1;
	*oduncler = (KitapOdunc*) realloc(*oduncler, sizeof(KitapOdunc) * (*bbCount));
	KitapOdunc* last = &(*oduncler)[*bbCount - 1];
	last->islemTarihi.gun = gun;
	last->islemTarihi.ay = ay;
	last->islemTarihi.yil = yil;
	strcpy(last->ogrID, student->ogrID);
	strcpy(last->etiketNo, ilkOrnek->etiketNo);
	last->islemTipi = islemTuru;
	writeKitapOdunc(bbPath, oduncler, bbCount);	
	
	printf("%s etiketli kitabi %s %s odunc aldi!\n", last->etiketNo, student->ad, student->soyad);
}


		
		
void turnInBook(KitapOdunc** oduncler, int* bbCount, const char* bbPath, Ogrenci** ogrenciler, Kitap** kitaplar,
	char** lateBooks, int* lateBookCount) {
	const int islemTuru = 1;
	Ogrenci* student;
	int gun, ay, yil;	
	if(!getInformation(ogrenciler, &student, &gun, &ay, &yil))
		return;
	
	printf("Lutfen teslim ettiginiz kitap orneginin etiket numarasini giriniz: ");
	char etiket[20];
	scanf("%s", etiket);
	
	// ISBN ve adet kontrolu
	char* p;
	char* dupEtiket = strdup(etiket);
	p = strtok(dupEtiket, "_");
	Kitap* book = findBook(kitaplar, cmpBookISBN, p);
	if(book == NULL) {
		printf("%s ISBN'li bir kitap bulunmamaktadir...\n", p);
		return;
	}
	
	p = strtok(NULL, "_");
	int num = atoi(p); // teslim edilen kitabin ornek sayisi
	if(num > book->adet) {
		printf("Bu kitabin %d. ornegi bulunmamaktadir...\n", num);
		return;
	}
	free(dupEtiket);
	
	// teslim edilen kitabi gecmiste o ogrenci odunc almis olmali
	KitapOdunc* head = *oduncler;
	int i;
	int found = 0;
	int lastDay, lastMonth, lastYear;
	for(i = *bbCount - 1; i >= 0 && found == 0; i--) {
		KitapOdunc* temp = &head[i];
		// en son odunc alan kisiyi bul
		if(temp->islemTipi == 0 && strcmp(temp->etiketNo, etiket) == 0) {
			found = 1;
			if(strcmp(temp->ogrID, student->ogrID) != 0){
				printf("Bu kitabi gecmiste sizden baskasi odunc almis, siz bu kitabi teslim edemezsiniz!\n");
				return;	
			}
			else {
				lastDay = temp->islemTarihi.gun;
				lastMonth = temp->islemTarihi.ay;
				lastYear = temp->islemTarihi.yil;
			}
		}
	}
	
	// teslim edilen kitap rafta olmamali
	KitapOrnek* ornek = getBookSample(kitaplar, book->ISBN, num);
	if(strcmp(ornek->durum, RAFTA_STR) == 0){
		printf("Bu kitap zaten rafta bulunmaktadir...\n");
		return;
	}
	
	// ceza var mi diye kontrol et varsa listeye ekle ve puanini azalt
	int dayDiff = (gun - lastDay) + (ay - lastMonth) * 30 + (yil - lastYear) * 365;
	if(dayDiff >= 15) {
		student->puan -= 10;
		printf("%s %s, %d gun geciktiginden ceza aldi\n", student->ad, student->soyad, dayDiff);
		saveLateTurnIn(lateBooks, lateBookCount, ornek->etiketNo);
	}
	
	// teslim et
	strcpy(ornek->durum, RAFTA_STR);
	
	// en sona ekle ve dosyayi yeniden yaz
	*bbCount = *bbCount + 1;
	*oduncler = (KitapOdunc*) realloc(*oduncler, sizeof(KitapOdunc) * (*bbCount));
	KitapOdunc* last = &(*oduncler)[*bbCount - 1];
	last->islemTarihi.gun = gun;
	last->islemTarihi.ay = ay;
	last->islemTarihi.yil = yil;
	strcpy(last->ogrID, student->ogrID);
	strcpy(last->etiketNo, etiket);
	last->islemTipi = islemTuru;
	writeKitapOdunc(bbPath, oduncler, bbCount);
	
	printf("%s etiket numarali kitabi %s %s teslim etti...\n", ornek->etiketNo, student->ad, student->soyad);
}




void addBook(Kitap** kitaplar, const char* pathKitaplar) {
	// ad elde et
	printf("Ekleme yapmak icin bir kitap adi giriniz: ");
	char kitapAdi[30];
	fflush(stdin);
	gets(kitapAdi);
	
	// adet elde et
	int adet;
	printf("Eklediginiz kitabin kac adedi olacak giriniz: ");
	scanf("%d", &adet);
	
	// rastgele ISBN uret
	char uniqueISBN[13];
	int i;
	do {
		for (i = 0; i < 12; i++) 
			uniqueISBN[i] = '0' + rand() % 10;
				
		uniqueISBN[12] = '\0';
	} while(findBook(kitaplar, cmpBookISBN, uniqueISBN) != NULL);
	
	// kitap olustur
	Kitap* add = (Kitap*) malloc(sizeof(Kitap));
	add->next = NULL;
	strcpy(add->ISBN, uniqueISBN);
	strcpy(add->kitapAdi, kitapAdi);
	add->adet = adet;
	createBookSamples(add);
	
	// listenin sonuna ekle, sort et, yazdir
	Kitap* last;
	for(last = *kitaplar; last->next != NULL; last = last->next);
	last->next = add;
	sortBooks(kitaplar);
	writeBooks(kitaplar, pathKitaplar);
}



void deleteBook(Kitap** kitaplar, const char* pathKitaplar, KitapYazar** ba, int* baCount, const char* baPath) {
	// ad elde et
	printf("Silmek icin bir kitap adi giriniz: ");
	char arama[30];
	fflush(stdin);
	gets(arama);
	Kitap* del = findBook(kitaplar, cmpBookName, arama);
	if(del == NULL) {
		printf("%s isimli bir kitap yoktur...\n", arama);
		return;
	}
	
	// listeden sil, listeyi sort et ve yaz
	if(del == *kitaplar) { // head siliniyorsa
		*kitaplar = del->next;
	}
	else {
		Kitap* prev;
		for(prev = *kitaplar; prev->next != del; prev = prev->next);
		prev->next = del->next;
	}
	sortBooks(kitaplar);
	writeBooks(kitaplar, pathKitaplar);
	
	// kitapyazardan da sil ve dosyaya yaz
	int i;
	int deleteCount = 0;
	for(i = 0; i < *baCount; i++){
		KitapYazar* cur = &(*ba)[i];
		if(strcmp(cur->ISBN, del->ISBN) == 0) 
			deleteCount++;
	}
	KitapYazar* arr = (KitapYazar*) malloc(sizeof(KitapYazar) * (*baCount - deleteCount));
	int index = 0;
	for(i = 0; i < *baCount; i++){
		KitapYazar* cur = &(*ba)[i];
		if(strcmp(cur->ISBN, del->ISBN) != 0) {
			KitapYazar* n = &arr[index];
			strcpy(n->ISBN, cur->ISBN);
			n->yazarID = cur->yazarID;
			index++;
		}
	}
	free(*ba);
	*ba = arr;
	*baCount = *baCount - deleteCount;
	writeKitapYazar(ba, baPath, baCount);
	
	free(del);
}


void updateBook(Kitap** kitaplar, const char* pathKitaplar) {
	// ad elde et
	printf("Guncellemek icin bir kitap adi giriniz: ");
	char arama[30];
	fflush(stdin);
	gets(arama);
	
	Kitap* update = findBook(kitaplar, cmpBookName, arama);
	if(update == NULL) {
		printf("%s isimli bir kitap yoktur...\n", arama);
		return;
	}
	
	printf("%s icin yeni bir kitap adi giriniz: ", update->kitapAdi);
	fflush(stdin);
	gets(&update->kitapAdi);
	sortBooks(kitaplar);
	writeBooks(kitaplar, pathKitaplar);
}



void pairBookAndAuthor(KitapYazar** baHead, int* baCount, Kitap** kitaplar, Yazar** yazarlar, const char* baPath) {
	Kitap* pairedBook;
	Yazar* pairedAuthor;
	
	// kitabi elde et
	printf("Esleme yapmak icin bir kitap adi giriniz: ");
	char kitapAdi[30];
	fflush(stdin);
	gets(kitapAdi);
	
	pairedBook = findBook(kitaplar, cmpBookName, kitapAdi);
	if(pairedBook == NULL) {
		printf("%s isimli bir kitap bulunmamaktadir...\n", kitapAdi);
		return;
	}
	
	// yazari elde et
	printf("Esleyeceginiz yazarin adini giriniz: ");
	char ad[30];
	scanf("%s", ad);
	printf("Esleyeceginiz yazarin soyadini giriniz: ");
	char soyad[30];
	scanf("%s", soyad);
	
	Yazar* yazarPrev; // kullanilmayacak ancak fonksiyonda bulunuyor
	findAuthorByFullname(yazarlar, &pairedAuthor, &yazarPrev, ad, soyad);
	if(pairedAuthor == NULL) {
		printf("%s %s isimli bir yazar bulunmamaktadir...\n", ad, soyad);
		return;
	}
	
	// zaten var mi diye kontrol et
	KitapYazar* arr = *baHead;
	int i;
	for(i = 0; i < *baCount; i++){
		KitapYazar* cur = &arr[i];
		if(cur->yazarID == pairedAuthor->yazarID && strcmp(cur->ISBN, pairedBook->ISBN) == 0) {
			printf("Boyle bir esleme zaten bulunmaktadir!\n");
			return;
		}
	}
	
	// esleme yap
	*baCount = *baCount + 1;
	arr = (KitapYazar*) realloc(arr, sizeof(KitapYazar) * (*baCount));
	KitapYazar* eklenen = &arr[*baCount - 1];
	eklenen->yazarID = pairedAuthor->yazarID;
	strcpy(eklenen->ISBN, pairedBook->ISBN);
	
	writeKitapYazar(baHead, baPath, baCount);
}


void updateBooksAuthor(Kitap** kitaplar, Yazar** yazarlar, KitapYazar** kitapYazar, int* count, const char* yazarlarPath) {
	printf("Yazar guncellemesi yapmak icin bir kitap adi giriniz: ");
	char arama[30];
	fflush(stdin);
	gets(arama);

	Kitap* searchedBook = findBook(kitaplar, cmpBookName, arama);
	if(searchedBook == NULL) {
		printf("%s aramasiyla bir kitap bulunamadi\n", arama);
		return;
	}
	
	int i;
	int wantTo = 0;
	for(i = 0; i < *count; i++) {
		KitapYazar* cur = &(*kitapYazar)[i];
		if(strcmp(searchedBook->ISBN, cur->ISBN) == 0){
			Yazar* yazar = findAuthorByID(yazarlar, cur->yazarID);
			
			printf("%s %s isimli yazari guncellemek istiyor musunuz (1: evet, 0: hayir): ", 
				yazar->yazarAd, yazar->yazarSoyad);
			scanf("%d", &wantTo);
			printf("\n");
			if(wantTo != 0){
				updateGivenAuthor(yazarlar, yazar->yazarAd, yazar->yazarSoyad, yazarlarPath);
			}
		}
	}
}
				


void updateGivenAuthor(Yazar** head, char* ad, char* soyad, const char* path) {
	Yazar *toUpdate, *prev;
	findAuthorByFullname(head, &toUpdate, &prev, ad, soyad);
	if(toUpdate == NULL){
		printf("%s %s isimli yazar bulunamadi...\n", ad, soyad);
		return;
	}
	
	// listede degisiklik
	char yeniAd[30], yeniSoyad[30];
	printf("Yeni ad giriniz: ");
	scanf("%s", yeniAd);
	printf("Yeni soyad giriniz: ");
	scanf("%s", yeniSoyad);
	printf("\n");
	
	// Eger boyle birisi varsa degistirme yapma
	Yazar* temp;
	for(temp = *head; temp != NULL; temp = temp->next){
		if(authorEqualsTo(temp, yeniAd, yeniSoyad)){
			printf("%s %s isimli yazar zaten veritabaninda bulunmakta...\n", yeniAd, yeniSoyad);
			return;
		}
	}
	
	// Yoksa degisikligi yap
	strcpy(toUpdate->yazarAd, yeniAd);
	strcpy(toUpdate->yazarSoyad, yeniSoyad);
		
	// dosyaya da yansitilmali
	char idStr[50];
	sprintf(idStr, "%d", toUpdate->yazarID);
	char* authStr = authorToString(toUpdate);
	filterFile(path, 2, strNotEqual, idStr, authStr);
	free(authStr);
}


void updateAuthor(Yazar** head, const char* path) {
	printf("Guncelleyeceginiz yazarin adini giriniz: ");
	char ad[30];
	scanf("%s", ad);
	printf("Guncelleyeceginiz yazarin soyadini giriniz: ");
	char soyad[30];
	scanf("%s", soyad);
	
	updateGivenAuthor(head, ad, soyad, path);
}




void deleteAuthor(Yazar** head, const char* pathYazarlar, KitapYazar** ba, const char* baPath, int* baCount) {
	printf("Sileceginiz yazarin adini giriniz: ");
	char ad[30];
	scanf("%s", ad);
	printf("Sileceginiz yazarin soyadini giriniz: ");
	char soyad[30];
	scanf("%s", soyad);
	
	Yazar *del, *prev;
	findAuthorByFullname(head, &del, &prev, ad, soyad);
	
	if(del == NULL){
		printf("%s %s isimli bir yazar bulunamadi...\n", ad, soyad);
		return;
	}

	// yazari kitapYazar dosyasindan da silme 	
	int i;
	for(i = 0; i < *baCount; i++){
		KitapYazar* cur = &((*ba)[i]);
		if(cur->yazarID == del->yazarID){
			cur->yazarID = -1;
		}
	}
	writeKitapYazar(ba, baPath, baCount);
 
	// listeden silme
	if (del == *head) 
		*head = del->next;
	else 
		prev->next = del->next;
	
	// dosyadan silme
	char idStr[MAX_LEN];
	sprintf(idStr, "%d", del->yazarID);
	filterFile(pathYazarlar, 2, strNotEqual, idStr, NULL);
	free(del);
}




void addAuthor(Yazar** yazarlar, const char* path) {
	Yazar* add = (Yazar*) malloc(sizeof(Yazar));
	add->next = NULL;
	
	// AD VE SOYAD GIRDISI
	printf("Yazar kaydi icin Ad giriniz: ");
	scanf("%s", add->yazarAd);
	printf("\n");
	printf("Yazar kaydi icin Soyad giriniz: ");
	scanf("%s", add->yazarSoyad);
	printf("\n");
	
	// Eger boyle birisi varsa ekleme yapma
	Yazar* temp;
	for(temp = *yazarlar; temp != NULL; temp = temp->next){
		if(authorEqualsTo(temp, add->yazarAd, add->yazarSoyad)) {
			printf("%s %s isimli yazar zaten veritabaninda bulunmakta...\n", add->yazarAd, add->yazarSoyad);
			free(add);
			return;
		}
	}
	
	// son ID'nin 1 fazlasi olmali
	for(temp = *yazarlar; temp->next != NULL; temp = temp->next);
	add->yazarID = temp->yazarID + 1;
	
	// en sona gidip ekle
	temp->next = add;
	
	// DOSYANIN SONUNA YENI KAYDI EKLEME
	FILE* file = openFile(path, "a+");
	char* str = authorToString(add);
	fprintf(file, "\n%s", str);
	free(str);
	fclose(file);
}



void addStudent(Ogrenci** head, const char* path) {
	Ogrenci* newOgr = (Ogrenci*) malloc(sizeof(Ogrenci));
	
	// ID GIRDISI, BENZERSIZ OLSUN DIYE KONTROL YAPIYORUM
	printf("Kayit icin ID giriniz: ");
	scanf("%s", newOgr->ogrID);
	printf("\n");
	
	Ogrenci* taken;
	while( (taken = findStudent(head, cmpStudentID, newOgr->ogrID)) != NULL ){
		printf("%s ID'si %s %s tarafindan alinmistir, baska ID giriniz: ", newOgr->ogrID, taken->ad, taken->soyad);
		scanf("%s", newOgr->ogrID);
		printf("\n");
	}
	
	// AD VE SOYAD GIRDISI
	printf("Kayit icin Ad giriniz: ");
	scanf("%s", newOgr->ad);
	printf("\n");
	printf("Kayit icin Soyad giriniz: ");
	scanf("%s", newOgr->soyad);
	printf("\n");
	newOgr->puan = 100;
	
	// DOSYANIN SONUNA YENI KAYDI EKLEME
	FILE* file = openFile(path, "a+");
	char* str = studentToString(newOgr);
	fprintf(file, "\n%s", str);
	free(str);
	fclose(file);

	// PROGRAMDAKI CIFT YONLU LINKLI LISTENIN SONUNA YENI KAYDI EKLEME
	newOgr->next = NULL;

	if (*head == NULL) {
		newOgr->prev = NULL;
		*head = newOgr;
		return;
	}

	Ogrenci* last;
	for(last = *head; last->next != NULL; last = last->next);
	last->next = newOgr;
	newOgr->prev = last;
}



void deleteStudent(Ogrenci** head, const char* path) {
	printf("Sileceginiz ogrencinin ID'sini giriniz: ");
	char id[9];
	scanf("%s", id);
	
	Ogrenci* del = findStudent(head, cmpStudentID, id);
	if(del == NULL){
		printf("%s ID'li ogrenci bulunamadi...\n", id);
		return;
	}
 
		// listeden silme
	if (*head == del){
		*head = del->next;
	}
	if (del->next != NULL) {
		del->next->prev = del->prev;
	}
	if (del->prev != NULL){
		del->prev->next = del->next;
	}
	free(del);
	
	// dosyadan silme
	filterFile(path, 3, strNotEqual, id, NULL);
}



void updateStudent(Ogrenci** ogrenciler, const char* path) {
	printf("Guncelleyeceginiz ogrencinin ID'sini giriniz: ");
	char id[9];
	scanf("%s", id);
	
	Ogrenci* toUpdate = findStudent(ogrenciler, cmpStudentID, id);
	if(toUpdate == NULL){
		printf("%s ID'li ogrenci bulunamadi...\n", id);
		return;
	}
		
	// listede degisiklik
	printf("Yeni ad giriniz: ");
	scanf("%s", toUpdate->ad);
	printf("Yeni soyad giriniz: ");
	scanf("%s", toUpdate->soyad);
	printf("\n");
	
	// dosyaya da yansitilmali
	char* studStr = studentToString(toUpdate);
	filterFile(path, 3, strNotEqual, id, studStr);
	free(studStr);
}



void searchStudent(Ogrenci** head, KitapOdunc** oduncler, int* borrowedCount){
	printf("Arama yapmak icin ID, ad veya soyad giriniz: ");
	char str[30];
	scanf("%s", str);
	
	Ogrenci* ref = *head;
	int found = 0;
	while(ref != NULL){
		if(strcmp(ref->ogrID, str) == 0 || strcmp(ref->ad, str) == 0 || strcmp(ref->soyad, str) == 0) {
			found = 1;
			printStudent(ref);
			
			printf("\nOgrencinin odunc ve teslim hareketleri:\n");
			int j;
			int printed = 0;
			for(j = 0; j < *borrowedCount; j++){
				KitapOdunc* od = &(*oduncler)[j];
				if(strcmp(od->ogrID, ref->ogrID) == 0) {
					printed = 1;
					printf("%s etiketli kitabi %d.%d.%d tarihinde %s.\n", od->etiketNo, 
						od->islemTarihi.gun, od->islemTarihi.ay, od->islemTarihi.yil,
						od->islemTipi == 0 ? "odunc almis" : "teslim etmis");
				}
			}
			if(!printed){
				printf("herhangi bir hareket yok...\n");
			}
			
		}
		ref = ref->next;
	}
	if(!found)
		printf("Ogrenci bulunamadi...\n");
}



void searchBooks(Kitap** kitaplar){
	printf("Arama yapmak icin bir kitap adi giriniz: ");
	char arama[30];
	scanf("%s", arama);
	
	printAllBookInfo(kitaplar, arama);
}


void showAuthorInfo(Yazar** yazarlar, KitapYazar** ba, int* baCount, Kitap** kitaplar) {
	printf("Arama yapmak icin bir yazar adi giriniz: ");
	char arama[30];
	scanf("%s", arama);
	
	Yazar* ref = *yazarlar;
	int i;
	int yazarBulundu = 0;
	while(ref != NULL){
		if(strcmp(ref->yazarAd, arama) == 0){ // eger aranan yazar ise
			yazarBulundu = 1;
			printf("\n%s %s yazari sunlari yazdi:\n", ref->yazarAd, ref->yazarSoyad);
			int yazmis = 0;
			for(i = 0; i < *baCount; i++){ // yazilan her kitap icin
				KitapYazar* cur = &(*ba)[i];
				if(ref->yazarID == cur->yazarID){ // eger kitap bu yazar tarafindan yazildi ise
					yazmis = 1;
					Kitap* kitap = findBook(kitaplar, cmpBookISBN, cur->ISBN); // o kitabin bilgilerini elde et
					printAllBookInfo(kitaplar, kitap->kitapAdi); // o kitabin her bilgisini yaz
				}
			}
			if(!yazmis)
				printf("%s %s yazari hicbir kitap yazmamis...\n\n", ref->yazarAd, ref->yazarSoyad);
			printf("-------------\n");
		}
		ref = ref->next;
	}
	if(!yazarBulundu)
		printf("%s isimli bir yazar bulunamadi\n", arama);
}




///////////////////////////////////////////////////////
//////////////  PRINTLEME FONKSIYONLARI  //////////////
///////////////////////////////////////////////////////


void printAllBookInfo(Kitap** kitaplar, char* bookName){
	Kitap* temp = *kitaplar;
	int found = 0;
	int i;
	printf("\n");
	while(temp != NULL){
		if(strstr(temp->kitapAdi, bookName)){ // substring icin
			found = 1;
			printf("Kitap Adi: %s, ISBN: %s, Adet: %d\n", temp->kitapAdi, temp->ISBN, temp->adet);
			KitapOrnek* ornekHead = temp->head;
			for(i = 0; i < temp->adet; i++){
				printf("\tOrnek #%d \t Etiket: %s, Durum: %s\n", i+1, ornekHead->etiketNo, ornekHead->durum);
				ornekHead = ornekHead->next;
			}
			printf("\n");
		}
		temp = temp->next;
	}
	if(!found)
		printf("Kitap bulunamadi...\n");
}

void printStudent(Ogrenci* n){
	printf("ID: %s, Ad: %s, Soyad: %s, Puan: %d\n", n->ogrID, n->ad, n->soyad, n->puan);
}

void printAuthor(Yazar* n){
	printf("%d,%s,%s\n", n->yazarID, n->yazarAd, n->yazarSoyad);
}

void listAllStudents(Ogrenci** ogrenciler) {
	Ogrenci* head;
	for(head = *ogrenciler; head != NULL; head = head->next)
		printStudent(head);
}

void listAllAuthors(Yazar** yazarlar){
	Yazar* head;
	for(head = *yazarlar; head != NULL; head = head->next)
		printAuthor(head);
}

char* studentToString(Ogrenci* node){
	char* str = (char*) malloc(MAX_LEN);
	sprintf(str, "%s,%s,%s,%d", node->ogrID, node->ad, node->soyad, node->puan);
	return str;
}

char* authorToString(Yazar* node){
	char* str = (char*) malloc(MAX_LEN);
	sprintf(str, "%d,%s,%s", node->yazarID, node->yazarAd, node->yazarSoyad);
	return str;
}




/////////////////////////////////////////////////////////
/////////////////  DOSYA FONKSIYONLARI  /////////////////
/////////////////////////////////////////////////////////



void writeKitapOdunc(const char* path, KitapOdunc** oduncler, int* borrowedCount) {
	FILE* file = openFile(path, "w"); // silsin icerigini
	int i;
	for(i = 0; i < *borrowedCount; i++){
		KitapOdunc* ref = &(*oduncler)[i];
		fprintf(file, "%s,%s,%d,%d.%d.%d", ref->etiketNo, ref->ogrID, ref->islemTipi, 
			ref->islemTarihi.gun, ref->islemTarihi.ay, ref->islemTarihi.yil);
		if(i + 1 != *borrowedCount)
			fprintf(file, "\n");
	}
	fclose(file);
}



void writeBooks(Kitap** head, const char* path) {
	FILE* file = openFile(path, "w"); // "w" ile acinca icerigi silinir

	Kitap* ref = *head;
	while(ref != NULL){
		fprintf(file, "%s,%s,%d", ref->kitapAdi, ref->ISBN, ref->adet);
		if(ref->next != NULL) // sonuncu degilse satir atlama koy
			fprintf(file, "\n");
		ref = ref->next;
	}
	
	fclose(file);
}


void writeKitapYazar(KitapYazar** head, const char* path, int* count){
	FILE* file = openFile(path, "w"); // "w" ile acinca icerigi silinir
	
	int i;
	for(i = 0; i < *count; i++){
		KitapYazar* cur = &((*head)[i]);
		fprintf(file, "%s,%d", cur->ISBN, cur->yazarID);
		if(i + 1 != *count) // sonuncu degilse satir atlama koy
			fprintf(file, "\n");
	}
	fclose(file);
}


KitapYazar* readBookAuthor(Kitap** bookHead, Yazar** authorHead, const char* bookAuthorPath, int* bookAuthorCount) {
	FILE* file = openFile(bookAuthorPath, "r");
	
	// bos olmayan satir sayisini elde et
	char satir[MAX_LEN];
	int lineCount = 0;
	while (fgets(satir, MAX_LEN, file)) {
		if(strlen(satir) > 1) lineCount++;
	}
	rewind(file);
		
	// dosyayi yeniden okuyup diziyi olustur
	KitapYazar* arr = (KitapYazar*) malloc(sizeof(KitapYazar) * lineCount);
	int index = 0;
	char* p;
	int i;
	char line[MAX_LEN];
	int errCount = 0;
	while (fgets(line, MAX_LEN, file)) {
		if(strlen(line) > 1) {
			p = strtok(line, ",");
			i = 0;
			while(p != NULL){
				KitapYazar* ba = &arr[index];
				if(i == 0) {
					if(findBook(bookHead, cmpBookISBN, p) == NULL){
						printf("Uyari: %s ISBN'li bir kitap bulunmamaktadir\n", p);
							
						errCount++;
						strcpy(ba->ISBN, "-1");
						ba->yazarID = -1;
						i = 9999; // oburune bakmaya gerek yok
					} 
					else {
						strcpy(ba->ISBN, p);
					}
				}
				if(i == 1) {
					int id = atoi(p);
					if(findAuthorByID(authorHead, id) == NULL){
						printf("Uyari: %d ID'li bir yazar bulunmamaktadir\n", id);
							
						errCount++;
						strcpy(ba->ISBN, "-1");
						ba->yazarID = -1;
						i = 9999; // oburune bakmaya gerek yok
					}
					else ba->yazarID = id;
				}
			
				i++;
				p = strtok(NULL, ",");
			}
			index++;
		}
	}
	fclose(file);
	
	// hatali olanlari sil gitsin
	KitapYazar* actualArr = (KitapYazar*) malloc(sizeof(KitapYazar) * (lineCount - errCount));
	int actualIndex = 0;
	for(i = 0; i < lineCount; i++){
		KitapYazar* cur = &arr[i];
		if(strcmp(cur->ISBN, "-1") != 0 && cur->yazarID != -1) {
			KitapYazar* act = &actualArr[actualIndex];
			strcpy(act->ISBN, cur->ISBN);
			act->yazarID = cur->yazarID;
			actualIndex++;
		}
	}
	*bookAuthorCount = lineCount - errCount;
	free(arr);
	
	return actualArr;
}



KitapOdunc* readBorrowed(const char* path, int* borrowedCount) {
	FILE* file = openFile(path, "a+");
	rewind(file);

	// bos olmayan satir sayisini elde et
	char satir[MAX_LEN];
	*borrowedCount = 0;
	while (fgets(satir, MAX_LEN, file)) {
		if(strlen(satir) > 3) *borrowedCount = *borrowedCount + 1;
	}
	rewind(file);
	
	// dosyayi yeniden okuyup diziyi olustur
	KitapOdunc* arr = (KitapOdunc*) malloc(sizeof(KitapOdunc) * (*borrowedCount));
	int index = 0;
	char* p;
	int i;
	char line[MAX_LEN];
	while (fgets(line, MAX_LEN, file)) {
		if(strlen(line) > 3) {
			p = strtok(line, ",");
			i = 0;
			while(p != NULL){
				KitapOdunc* odunc = &arr[index];
	
				if(i == 0)
					strcpy(odunc->etiketNo, p);
				if(i == 1)
					strcpy(odunc->ogrID, p);
				if(i == 2) {
					if(strcmp(p, "0") == 0)
						odunc->islemTipi = 0;
					if(strcmp(p, "1") == 0)
						odunc->islemTipi = 1;
				}
				if(i == 3) {
					p = strtok(p, "\n");
					int x, y, z;
					fflush(stdin);
					sscanf(p, "%d.%d.%d", &x, &y, &z);
					odunc->islemTarihi.gun = x;
					odunc->islemTarihi.ay = y;
					odunc->islemTarihi.yil = z;
				}
				
				i++;
				p = strtok(NULL, ",");
			}
			index++;
		}
	}
	fclose(file);
	
	return arr;
}





void processBorrowed(Kitap** kitaplar, Ogrenci** ogrenciler, KitapOdunc** oduncler, int* borrowedCount,
	char** lateBooks, int* lateBookCount) {
	int i;
	for(i = 0; i < *borrowedCount; i++){
		KitapOdunc* ref = &(*oduncler)[i];
		char etiket[20];
		char ISBN[20];
		int num;
		strcpy(etiket, ref->etiketNo);
		char* p = strtok(etiket, "_");
		strcpy(ISBN, p);
		p = strtok(NULL, "_");
		num = atoi(p);
		KitapOrnek* ornek = getBookSample(kitaplar, ISBN, num);
		if(ref->islemTipi == 0) { // ODUNC
			if(strcmp(ornek->durum, RAFTA_STR) == 0){
				strcpy(ornek->durum, ref->ogrID);
			}
			else {
				printf("%s ISBN'li ornek rafta olmadigindan odunc alinamaz\n", ornek->etiketNo);
			}
		}
		else if(ref->islemTipi == 1){  // TESLIM
			Ogrenci* student = findStudent(ogrenciler, cmpStudentID, ref->ogrID);
			
			// gec teslim ettiyse ceza verilecek
			KitapOdunc* head = *oduncler;
			int i;
			int found = 0;
			int lastDay, lastMonth, lastYear;
			for(i = *borrowedCount - 1; i >= 0 && found == 0; i--) {
				KitapOdunc* temp = &head[i];
				// en son odunc alan kisiyi bul
				if(temp->islemTipi == 0 && strcmp(temp->etiketNo, etiket) == 0) {
					found = 1;
					if(strcmp(temp->ogrID, student->ogrID) != 0){
						printf("Bu kitabi gecmiste sizden baskasi odunc almis, siz bu kitabi teslim edemezsiniz!\n");
					}
					else {
						lastDay = temp->islemTarihi.gun;
						lastMonth = temp->islemTarihi.ay;
						lastYear = temp->islemTarihi.yil;
					}
				}
			}
			int gun = ref->islemTarihi.gun;
			int ay = ref->islemTarihi.ay;
			int yil = ref->islemTarihi.yil;
			int dayDiff = (gun - lastDay) + (ay - lastMonth) * 30 + (yil - lastYear) * 365;
			if(dayDiff >= 15) {
				student->puan -= 10;
				printf("%s %s, %d gun geciktiginden ceza aldi\n", student->ad, student->soyad, dayDiff);
				saveLateTurnIn(lateBooks, lateBookCount, ornek->etiketNo);
			}
		
			if(strcmp(ornek->durum, RAFTA_STR) != 0){
				strcpy(ornek->durum, RAFTA_STR);
			}
			else {
				printf("%s ISBN'li ornek rafta iken nasil teslim edilebilir ki?\n", ornek->etiketNo);
			}
		}
		
	}
}





Yazar* readAuthors(const char* path) {
	FILE* file = openFile(path, "r");
	
	Yazar* head = NULL;
	char line[MAX_LEN];
	while (fgets(line, MAX_LEN, file)) {
		if(strlen(line) > 2) {
			Yazar* yazar = (Yazar*) malloc(sizeof(Yazar));
			yazar->next = NULL;
			
			int i = 0;
			char* p = strtok(line, ",");
			while(p != NULL){
				if(i == 0) yazar->yazarID = atoi(p);
				if(i == 1) strcpy(yazar->yazarAd, p);
				if(i == 2) {
					p = strtok(p, "\n"); // son terimdeki \n'i kaldirir
					strcpy(yazar->yazarSoyad, p);
				}
				
				i++;
				p = strtok(NULL, ",");
			}
			
			
			if(head == NULL){
				head = yazar;
			}
			else{
				Yazar* last = head;
				while(last->next != NULL){
					last = last->next;
				}
				
				last->next = yazar;
			}	
		}
	}
	
	fclose(file);
	return head;
}



Ogrenci* readStudents(const char* path) {
	FILE* file = openFile(path, "r");
	
	Ogrenci* head = NULL;
	char line[MAX_LEN];
	while (fgets(line, MAX_LEN, file)) {
		// bilgi olan satirlarda 4 bilgi oldugundan 3 virgul oluyor, bu yuzden lineLen>3 mu diye baktim
		if(strlen(line) > 3){
			// satirda bulunan kisiyi oku ve ogr nesnesini olustur
			Ogrenci* ogr = (Ogrenci*) malloc(sizeof(Ogrenci));
		
			int i = 0;
			char* p = strtok(line, ",");
			while(p != NULL){
				if(i == 0) strcpy(ogr->ogrID, p);
				if(i == 1) strcpy(ogr->ad, p);
				if(i == 2) strcpy(ogr->soyad, p);
				if(i == 3) ogr->puan = atoi(p);
				
				i++;
				p = strtok(NULL, ",");
			}
			
			// en sona ekle
			if(head == NULL){
				ogr->next = NULL;
				ogr->prev = NULL;
				head = ogr;
			}
			else{
				Ogrenci* last = head;
				while(last->next != NULL){
					last = last->next;
				}
				
				last->next = ogr;
				ogr->prev = last;
				ogr->next = NULL;
			}	
		}
	}
	
	fclose(file);
	return head;
}




Kitap* readBooks(const char* path) {
	FILE* file = openFile(path, "r");
	
	Kitap* head = NULL;
	char line[MAX_LEN];
	while (fgets(line, MAX_LEN, file)) {
		if(strlen(line) > 2){
			Kitap* kitap = (Kitap*) malloc(sizeof(Kitap));
			kitap->next = NULL;
		
			int i = 0;
			char* p = strtok(line, ",");
			while(p != NULL){
				if(i == 0) strcpy(kitap->kitapAdi, p);
				if(i == 1) strcpy(kitap->ISBN, p);
				if(i == 2) kitap->adet = atoi(p);
		
				i++;
				p = strtok(NULL, ",");
			}
			
			createBookSamples(kitap);
			
			// en sona ekle
			if(head == NULL){
				head = kitap;
			}
			else{
				Kitap* last;
				for(last = head; last->next != NULL; last = last->next);
				last->next = kitap;
			}	
		}
	}
		
	fclose(file);
	
	sortBooks(&head);
	writeBooks(&head, path);
	return head;
}


void listAvailableBookSamples(Kitap** head) {
	Kitap* ref = *head;
	printf("Rafta olan kitaplar sunlardir: \n");
	while(ref != NULL){
		KitapOrnek* ornekHead = ref->head;
		while(ornekHead != NULL) {
			if(strcmp(ornekHead->durum, RAFTA_STR) == 0){
				printf("\t%s, %s\n", ref->kitapAdi, ornekHead->etiketNo);
			}			
			ornekHead = ornekHead->next;
		}		
		ref = ref->next;
	}
}



// degerleri olusturulmus bir kitap gonderilmeli, adet ve ISBN bundan okunacak!
void createBookSamples(Kitap* kitap){
	KitapOrnek* ornekHead = NULL;
	int i;
	for(i = 0; i < kitap->adet; i++){
		KitapOrnek* ornek = (KitapOrnek*) malloc(sizeof(KitapOrnek));
		ornek->next = NULL;
		sprintf(ornek->etiketNo, "%s_%d", kitap->ISBN, i+1);
		strcpy(ornek->durum, RAFTA_STR);
		
		// listeye ekle
		if(ornekHead == NULL) {
			ornekHead = ornek;
		}
		else {
			KitapOrnek* last;
			for(last = ornekHead; last->next != NULL; last = last->next);
			last->next = ornek;
		}
	}
	kitap->head = ornekHead;
}








///////////////////////////////////////////////////
//////////////  HELPER FONKSIYONLAR  //////////////
///////////////////////////////////////////////////



// split-sort-merge ile kitap linkedlist'ini siralar
void sortBooks(Kitap** head) {
	if (*head == NULL || (*head)->next == NULL) {
		return;
	}
	
	Kitap* sorted = NULL;
	Kitap* unsorted = *head;
	
	while (unsorted != NULL) {
		Kitap* current = unsorted;
		unsorted = unsorted->next;
		
		Kitap* prev = NULL;
		Kitap* cur = sorted;
		while (cur != NULL && 
			(strcmp(current->kitapAdi, cur->kitapAdi) > 0 || 
				(strcmp(current->kitapAdi, cur->kitapAdi) == 0 && strcmp(current->ISBN, cur->ISBN) > 0))) {
			prev = cur;
			cur = cur->next;
		}
		
		if (prev == NULL) {
			sorted = current;
		}
		else {
			prev->next = current;
		}
		current->next = cur;
	}
	
	*head = sorted;
}



int getNumberInRange(int a, int b) {
	int val;
	do{
		printf("Lutfen bir secim yapiniz: ");
		scanf("%d", &val);
		printf("\n");
	} while(!(a <= val && val <= b));
	return val;
}



/* 
	bu fonksiyon bir dosyayi guncellerken ve silerken kullanilir
	path ile dosya acilir ve lineCommaCount ile satirin bos olup olmadigi belirlenir
	eger girilen id ile satirin ilk virgule dek olan kismi cmpFunc'ta true donuyorsa satira dokunulmaz
	ancak ayni degilse verilen str parametresi "\n" ile birlikte o satir yerine yazilir
	bu str NULL olarak verilirse de hicbir sey yapilmaz yani satir atlanir ve silinmis olur 
*/
void filterFile(const char* path, const int lineCommaCount, int(*cmpFunc)(char*,char*), char* id, char* str){
	FILE* file = openFile(path, "r");
	char line[MAX_LEN];
	char newContent[MAX_LEN] = {0};
	char* p;
	char* dupLine;
	while (fgets(line, MAX_LEN, file)) {
		if(strlen(line) > lineCommaCount){ // not empty
			dupLine = strdup(line);
			p = strtok(dupLine, ",");
			if(cmpFunc(p, id)) {
				strcat(newContent, line);
			}
			else {
				if(str != NULL){ // silme durumu
					char changedLine[MAX_LEN] = {0};
					strcat(changedLine, str);
					strcat(changedLine, "\n");
					strcat(newContent, changedLine);
				}
			}
			free(dupLine);
		}
	}
	fclose(file);
	file = openFile(path, "w"); // "w" ile acinca icerigi silinir
	fputs(newContent, file);
	fclose(file);
}


FILE* openFile(const char* path, const char* mode) {
	FILE* file = fopen(path, mode);
	if(file == NULL) {
		printf("%s dizininde bulunan dosya %s modunda acilamadi", path, mode);
		exit(1);
	}
	return file;
}



int cmpStudentID(Ogrenci* ref, char* id) {
	return strcmp(ref->ogrID, id) == 0;
}


Ogrenci* findStudent(Ogrenci** head, int(*cmpStudent)(Ogrenci*, char*), char* str){
	Ogrenci* ref = *head;
	while(ref != NULL){
		if(cmpStudentID(ref, str)){
			return ref;
		}
		ref = ref->next;
	}
	return NULL;
}


// found ve prev parametreleri uzerinden istenilen node'u ve ondan bir onceki node'u (varsa) doner
void findAuthorByFullname(Yazar** head, Yazar** found, Yazar** prev, char* ad, char* soyad){
	*found = *head;
	*prev = NULL;
	while((*found) != NULL && !authorEqualsTo(*found, ad, soyad)){
		*prev = *found;
		*found = (*found)->next;
	}
}



int strNotEqual(char* p, char* id){
	return strcmp(p, id) != 0;
}

int authorEqualsTo(Yazar* yazar, char* ad, char* soyad){
	return strcmp(yazar->yazarAd, ad) == 0 && strcmp(yazar->yazarSoyad, soyad) == 0;
}

int cmpBookISBN(Kitap* book, char* ISBN){
	return strcmp(book->ISBN, ISBN) == 0;
}

int cmpBookName(Kitap* book, char* kitapAdi){
	return strcmp(book->kitapAdi, kitapAdi) == 0;
}


Kitap* findBook(Kitap** head, int(*cmpFunc)(Kitap*, char*), char* cmpStr){
	Kitap* ref = *head;
	while(ref != NULL){
		if(cmpFunc(ref, cmpStr))
			return ref;
		ref = ref->next;
	}
	return ref;
}

Yazar* findAuthorByID(Yazar** head, int ID){
	Yazar* ref = *head;
	while(ref != NULL){
		if(ref->yazarID == ID)
			return ref;
		ref = ref->next;
	}
	return ref;
}


int isDateValid(int dd, int mm, int yy){
	int flag = 1;
	if(yy>=1900 && yy<=9999) {
		if(mm>=1 && mm<=12) {
			if((dd>=1 && dd<=31) && (mm==1 || mm==3 || mm==5 || mm==7 || mm==8 || mm==10 || mm==12))
				flag = 1;
			else if((dd>=1 && dd<=30) && (mm==4 || mm==6 || mm==9 || mm==11))
				flag = 1;
			else if((dd>=1 && dd<=28) && (mm==2))
				flag = 1;
			else if(dd==29 && mm==2 && (yy%400==0 ||(yy%4==0 && yy%100!=0)))
				flag = 1;
			else
				return 0;
		}
		else return 0;
	}
	else return 0;
	
	return flag;
}


KitapOrnek* getBookSample(Kitap** kitaplar, char* ISBN, int num) {
	Kitap* book = findBook(kitaplar, cmpBookISBN, ISBN);
	KitapOrnek* ornek = book->head;
	int i;
	for(i = 1; i < num; i++){
		ornek = ornek->next;
	}
	return ornek;
}



int getInformation(Ogrenci** ogrenciler, Ogrenci** ogrenci, int* gun, int* ay, int* yil){
	// ogrenci ID elde et ve kontrol et
	printf("Lutfen ogrenci ID'nizi giriniz: ");
	char ogrID[9];
	scanf("%s", ogrID);
	*ogrenci = findStudent(ogrenciler, cmpStudentID, ogrID);
	if(*ogrenci == NULL) {
		printf("%s ID'li bir ogrenci bulunmamaktadir...\n", ogrID);
		return 0;
	}
	
	// tarihi elde et
	printf("Lutfen islem tarihinizi giriniz (dd.mm.yyyy formatinda): ");
	scanf("%d.%d.%d", gun, ay, yil);
	if(isDateValid(*gun, *ay, *yil) == 0){
		printf("%d.%d.%d diye bir tarih olamaz!\n", gun, ay, yil);
		return 0;
	}
	
	return 1;
}



void saveLateTurnIn(char** lateTurnInISBN, int* count, char* etiket) {
	if(*lateTurnInISBN == NULL) {
		*count = 1;
		*lateTurnInISBN = (char*) malloc(sizeof(char) * (*count));
		strcpy(&(*lateTurnInISBN)[0], etiket);
	}
	else {
		*count = *count + 1;
		*lateTurnInISBN = (char*) realloc(*lateTurnInISBN, sizeof(char) * (*count));
		strcpy(&(*lateTurnInISBN)[*count - 1], etiket);
	}
}



