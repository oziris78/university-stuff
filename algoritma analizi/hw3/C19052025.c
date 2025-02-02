

/*
 * Algoritma Analizi - Ödev #3
 * Oğuzhan Topaloğlu, Ç19052025
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct HashmapEntry {
    char* name;
    char* type;
} HashmapEntry;


typedef struct Hashmap {
    HashmapEntry** table;
    int size;
    int debugEnabled;
} Hashmap;


// Hashmap fonksiyonları
int determineHashmapSize(char*, long, int);
void hashmapParseVariables(Hashmap*, char*, long);
void hashmapDeclareVariable(Hashmap*, char*, char*);
void hashmapRetrieveVariable(Hashmap*, char*);
Hashmap* newHashmap(int, int);
int hash(Hashmap*, char*, int*);
void hashmapInsert(Hashmap*, char*, char*);
HashmapEntry* hashmapLookup(Hashmap*, char*);
void hashmapPrint(Hashmap*);
void hashmapFree(Hashmap*);

// Helper fonksiyonlar
long getFileLength(const char*);
char* getFileContents(const char*, long);
int isSkipCharacter(char c);
int isVariableType(char*);
int isDelimiter(char);
int isPrime(int);



int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Kullanim: %s <dosya_adi> <NORMAL|DEBUG>\n", argv[0]);
        return -78;
    }

    // Dosya içeriğini okuma
    const char* fileName = argv[1];
    long textLength = getFileLength(fileName);
    char* text = getFileContents(fileName, textLength);
    
    // Debug bayrağı
    const char* mode = argv[2];
    int debugEnabled = (strcmp(mode, "DEBUG") == 0);

    // Hashmap işlemleri
    int hashmapSize = determineHashmapSize(text, textLength, debugEnabled);
    Hashmap* hashmap = newHashmap(hashmapSize, debugEnabled);
    hashmapParseVariables(hashmap, text, textLength);

    // Gerekliyse debugging yapma
    if(debugEnabled) {
        printf("DEBUG: Sembol tablosunun son hali:\n");
        hashmapPrint(hashmap);
    }

    // Belleği free etme
    hashmapFree(hashmap);
    free(text);

    return 0;
}



// -------------------------  HASHMAP FUNCTIONS  ------------------------- //


/**
 * @brief Verilen kod stringi üzerinde değişkenleri parse ederek hashmap'in sembol tablosuna ekler
 * 
 * @param hashmap Hashmap'in adresi.
 * @param text kod stringi
 * @param textLength kod stringinin uzunluğu
 */
void hashmapParseVariables(Hashmap* hashmap, char* text, long textLength) {
    char buff[1024] = {0};
    int i, j = 0, k, isInMain = 0;

    for (i = 0; i < textLength; i++) {
        char c = text[i];

        if(!isSkipCharacter(c)) {
            if(!isInMain) {
                if(c != ' ' || isVariableType(buff)) {
                    buff[j++] = c;
                    buff[j] = '\0';

                    // int main bulundu artık ciddi aramaya başlayabiliriz
                    if (strcmp(buff, "int main()") == 0) {
                        isInMain = 1;
                        j = 0;
                        memset(buff, '\0', sizeof(buff));
                    }
                }
            }
            else {
                if (c == ';') {
                    buff[j] = '\0';

                    char varInfo[64] = {0};
                    char lineType[64] = "unset";
                    int vi = 0;
                    for(k = 0; k < j; k++) {
                        char cb = buff[k];
                        if(cb != ' ') {
                            varInfo[vi++] = cb;

                            if(isVariableType(varInfo)) {
                                varInfo[vi+1] = '\0';
                                strcpy(lineType, varInfo);
                                vi = 0;
                                memset(varInfo, '\0', sizeof(varInfo));
                            }
                            else if(cb == '_') {
                                // underscore'u geç
                                k++; 
                                
                                // virgül gelmedikçe ya da satır bitmedikçe değişken ismini oku
                                while(k < j && buff[k] != ',') { 
                                    varInfo[vi++] = buff[k++];
                                }

                                // elde ettiğin verileri hashmap'ten geçir
                                if(strcmp(lineType, "unset") != 0) {
                                    // değişken tanımı yapılmış bu satırda
                                    hashmapDeclareVariable(hashmap, lineType, varInfo);
                                }
                                else {
                                    // değişken tanımı yapılmamış ancak erişim olabilir
                                    char token[64] = {0};
                                    int i, ti = 0; // Token index

                                    for (i = 0; i <= vi; i++) {
                                        char c = varInfo[i];

                                        if (isDelimiter(c)) {
                                            if (ti > 0) { // geçerli token var mı?
                                                token[ti] = '\0';

                                                // token geçerli bir değişken ismi mi?
                                                if (token[0] == '_') {
                                                    hashmapRetrieveVariable(hashmap, token);
                                                }

                                                ti = 0;
                                                memset(token, '\0', sizeof(token));
                                            }
                                        } 
                                        else if (isalnum(c) || c == '_') {
                                            token[ti++] = c;
                                        }
                                    }
                                }

                                // bir daha kullanılabilir diye resetle                           
                                vi = 0;
                                memset(varInfo, '\0', sizeof(varInfo));
                            }
                        }
                    }

                    j = 0;
                    memset(buff, '\0', sizeof(buff));
                } 
                else {
                    buff[j++] = c;
                }
            }
        }
    }
}


/**
 * @brief Verilen kod stringine göre uygun ve asal bir hashmap boyutu hesaplar
 * 
 * @param text kod içeriği
 * @param textLength text stringinin uzunluğu
 * @param debugEnabled debug bayrağı
 * @return int Hesaplanan hashmap boyutu.
 */
int determineHashmapSize(char* text, long textLength, int debugEnabled) {
    int hashmapSize = 0;
    char buff[1024] = {0};
    int i, j = 0, k, isInMain = 0;

    for (i = 0; i < textLength; i++) {
        char c = text[i];

        if(!isSkipCharacter(c)) {
            if(!isInMain) {
                if(c != ' ' || isVariableType(buff)) {
                    buff[j++] = c;
                    buff[j] = '\0';

                    // int main bulundu artık ciddi aramaya başlayabiliriz
                    if (strcmp(buff, "int main()") == 0) {
                        isInMain = 1;
                        j = 0;
                        memset(buff, '\0', sizeof(buff));
                    }
                }
            }
            else {
                if (c == ';') {
                    buff[j] = '\0';

                    char varInfo[64] = {0};
                    int typeFound = 0;
                    int vi = 0;
                    for(k = 0; k < j; k++) {
                        char cb = buff[k];
                        if(cb != ' ') {
                            varInfo[vi++] = cb; 

                            if(isVariableType(varInfo)) {
                                typeFound = 1;
                                vi = 0;
                                memset(varInfo, '\0', sizeof(varInfo));
                            }
                            else if(cb == '_') {
                                // underscore'u geç
                                k++; 
                                
                                // virgül gelmedikçe ya da satır bitmedikçe değişken ismini oku
                                while(k < j && buff[k] != ',') { 
                                    varInfo[vi++] = buff[k++];
                                }

                                // elde ettiğin verileri hashmap'ten geçir
                                if(typeFound) {
                                    // değişken tanımı yapılmış bu satırda
                                    hashmapSize++;
                                }
                                // else ... sayı yeterli diye ignore ettim
                                
                                // bir daha kullanılabilir diye resetle                           
                                vi = 0;
                                memset(varInfo, '\0', sizeof(varInfo));
                            }
                        }
                    }

                    j = 0;
                    memset(buff, '\0', sizeof(buff));
                } 
                else buff[j++] = c;
            }
        }
    }

    if(debugEnabled) {
        printf("DEBUG: Deklere edilmis degisken sayisi: %d\n", hashmapSize);
    }
    hashmapSize = 2*hashmapSize + 1;
    while(!isPrime(hashmapSize)) {
        hashmapSize++;
    }
    if(debugEnabled) {
        printf("DEBUG: Sembol tablosunun uzunlugu: %d\n", hashmapSize);
    }

    return hashmapSize;
}


/**
 * @brief Yeni bir değişkeni hashmap'e deklere eder
 * 
 * @param hashmap işlem yapılan hashmap
 * @param variableType değişkenin veri türü
 * @param variableName değişkenin ismi
 * 
 * @note Eğer aynı isimde bir değişken daha önce deklere edilmişse hata mesajı verilir.
 */
void hashmapDeclareVariable(Hashmap* hashmap, char* variableType, char* variableName) {
    HashmapEntry* entry = hashmapLookup(hashmap, variableName);

    if(entry != NULL) {
        /* Eğer aynı isimli daha önce deklere edilmiş bir değişken 
         yeniden deklere edilmek isteniyorsa hata mesajı verir. */
        printf("HATA: %s degiskeni daha once deklere edilmistir.\n", variableName);
    }
    else {
        hashmapInsert(hashmap, variableName, variableType);
    }
}


/**
 * @brief Hashmap'ten bir değişkenin değerini okumayı dener
 * 
 * @param hashmap işlem yapılan hashmap
 * @param variableName değişkenin ismi
 * 
 * @note Eğer belirtilen değişken daha önce deklere edilmemişse hata mesajı verilir.
 */
void hashmapRetrieveVariable(Hashmap* hashmap, char* variableName) {
    HashmapEntry* entry = hashmapLookup(hashmap, variableName);
    
    if(entry == NULL) {
        /* Program içinde kullanılan bir değişken sembol tablosunda 
        yoksa deklere edilmediğine dair hata mesajı verir */
        printf("HATA: %s degiskeni deklere edilmemistir.\n", variableName);
    }
}


/**
 * @brief Yeni bir hashmap oluşturur
 * 
 * @param hashmapSize Hashmap'in boyutu
 * @param debugEnabled Debug modunun etkin olup olmadığını belirten bayrak
 * @return Hashmap* Yeni oluşturulan hashmap
 */
Hashmap* newHashmap(int hashmapSize, int debugEnabled) {
    Hashmap* hashmap = (Hashmap*) malloc(sizeof(Hashmap));
    hashmap->size = hashmapSize;
    hashmap->debugEnabled = debugEnabled;
    hashmap->table = (HashmapEntry**) malloc(sizeof(HashmapEntry*) * hashmapSize);

    int i;
    for (i = 0; i < hashmapSize; i++) {
        hashmap->table[i] = NULL;
    }

    return hashmap;
}


/**
 * @brief Verilen string'e göre hashmap'te bir key indisi hesaplar
 * 
 * @param hashmap Hashmap'in adresi
 * @param str Anahtar olarak kullanılacak string.
 * @param keyHFirst Anahtarın ilk hesaplanan değerini döndürmek için kullanılan pointer (debug için)
 * @return int Hesaplanan indis değerini döner
 */
int hash(Hashmap* hashmap, char* str, int* keyHFirst) {
    const int hashmapSize = hashmap->size;
    int hornerKey = 0;
    int i = 0;
    for (i = 0; str[i] != '\0'; i++) {
        hornerKey = (hornerKey * 31 + str[i]) % hashmapSize;
    }

    int keyH1 = hornerKey % hashmapSize;
    int keyH2 = 1 + (hornerKey % (hashmapSize - 3));
    
    i = 0;
    int keyH = (keyH1 + i * keyH2) % hashmapSize;

    if(keyHFirst != NULL) {
        *keyHFirst = keyH;
    }

    while (hashmap->table[keyH] != NULL && strcmp(hashmap->table[keyH]->name, str) != 0) {
        i++;
        keyH = (keyH1 + i * keyH2) % hashmapSize;
    }

    return keyH;
}


/**
 * @brief Hashmap'e yeni bir entry ekler
 * 
 * @param hashmap entry eklenecek hashmap
 * @param name değişken adı
 * @param type veri türü
 */
void hashmapInsert(Hashmap* hashmap, char* name, char* type) {
    int keyHFirst;
    int index = hash(hashmap, name, &keyHFirst);

    if(hashmap->debugEnabled) {
        printf(
            "DEBUG: %s degiskeni icin ilk adres degeri: %d, son adres: %d\n", 
            name, keyHFirst, index
        );
    }

    hashmap->table[index] = (HashmapEntry*) malloc(sizeof(HashmapEntry));
    hashmap->table[index]->name = strdup(name);
    hashmap->table[index]->type = strdup(type);
}


/**
 * @brief Verilen isme sahip bir değişkeni hashmap'te arar
 * 
 * @param hashmap Arama yapılacak hashmap
 * @param name Aranacak değişkenin ismi
 * @return HashmapEntry* bulunan entry ya da bulunamazsa NULL
 */
HashmapEntry* hashmapLookup(Hashmap* hashmap, char* name) {
    int index = hash(hashmap, name, NULL);
    if (hashmap->table[index] != NULL && strcmp(hashmap->table[index]->name, name) == 0) {
        return hashmap->table[index];
    }
    return NULL;
}


/**
 * @brief Hashmap'in sembol tablosunu ekrana yazdırır
 * 
 * @param hashmap Yazdırılacak hashmap
 */
void hashmapPrint(Hashmap* hashmap) {
    printf("  Hashmap (Size: %d):\n", hashmap->size);
    int i;
    for (i = 0; i < hashmap->size; i++) {
        if (hashmap->table[i] != NULL) {
            printf("    - %d: %s %s;\n", i, hashmap->table[i]->type, hashmap->table[i]->name);
        } else {
            printf("    - %d: ..\n", i);
        }
    }
}


/**
 * @brief Hashmap'teki tüm dinamik olarak ayrılmış hafıza alanlarını free eder
 * 
 * @param hashmap Free edilecek hashmap
 */
void hashmapFree(Hashmap* hashmap) {
    if (hashmap == NULL) return;

    int i;
    for (i = 0; i < hashmap->size; i++) {
        if (hashmap->table[i] != NULL) {
            free(hashmap->table[i]->name);
            free(hashmap->table[i]->type);
            free(hashmap->table[i]);
        }
    }

    free(hashmap->table);
    free(hashmap);
}



// -------------------------  HELPER FUNCTIONS  ------------------------- //


/**
 * @brief Dosyanın uzunluğunu byte olarak hesaplar. Dosyada kaç karakter olduğunu belirtir.
 * 
 * @param fileName Uzunluğu hesaplanacak dosyanın adı
 * @return long Dosyanın uzunluğunu byte cinsinden döner
 */
long getFileLength(const char* fileName) {
    FILE* file = fopen(fileName, "rb");
    if (!file) {
        printf("Failed to open file: %s\n", fileName);
        exit(-78);
    }

    fseek(file, 0, SEEK_END);
    long textLength = ftell(file);
    fclose(file);
    return textLength;
}


/**
 * @brief Dosyanın içeriğini okur ve bir str olarak döner
 * 
 * @param fileName Okunacak dosyanın adı.
 * @param textLength Dosyanın uzunluğu
 * @return char* Dosyanın içeriğini tutan str döner
 */
char* getFileContents(const char* fileName, long textLength) {
    FILE* file = fopen(fileName, "rb");
    if (!file) {
        printf("Failed to open file: %s\n", fileName);
        exit(-78);
    }

    char* text = (char*) malloc(textLength + 1); // +1 çünkü '\0' eklenecek
    if (!text) {
        printf("Malloc failed!");
        exit(-78);
    }

    // Dosyanın tüm içeriğini text isimli değişkene oku
    size_t read_size = fread(text, 1, textLength, file);
    text[read_size] = '\0';
    fclose(file);
    return text;
}


/**
 * @brief Bir karakterin geçilebilir bir karakter olup olmadığını döner
 * 
 * @param c Kontrol edilecek karakter.
 * @return int karakter skiplenecekse 1 skiplenmeyecekse 0 döner
 */
int isSkipCharacter(char c) {
    return c == '\n' || c == '\r' || c == '\t' || c == '{' || c == '}';
}


/**
 * @brief Bir karakter dizisinin değişken tipi (int, float, char) olup olmadığını kontrol eder.
 * 
 * @param buff Kontrol edilecek karakter dizisi.
 * @return int eğer karakter dizisi bir değişken tipi (int, float, char) ise 1 değilse 0 döner
 */
int isVariableType(char* buff) {
    return strcmp(buff, "int") == 0 || strcmp(buff, "float") == 0 || strcmp(buff, "char") == 0;
}


/**
 * @brief Bir karakterin ayırıcı (delimiter) olup olmadığını döner
 *  
 * @param c Kontrol edilecek karakter
 * @return int delimiter ise 1 değilse 0 döner
 */
int isDelimiter(char c) {
    return c == ' ' || c == '=' || c == '+' || c == '-' || 
        c == '*' || c == '/' || c == ';' || c == '\0';
}


/**
 * @brief Bir sayının asal olup olmadığını döner
 * 
 * @param num Kontrol edilecek sayı
 * @return int asalsa 1 değilse 0 döner
 */
int isPrime(int num) {
    if (num <= 1) return 0;
    
    int i;
    for (i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) {
            return 0;
        }
    }

    return 1;
}


