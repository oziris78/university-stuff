
/*
 * Algoritma Analizi - Ödev #2
 * Oğuzhan Topaloğlu, Ç19052025
*/

#define INFINITY 2147483647  

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

///////////////////////////////////////////////////////////////////

// Fonksiyonlar
void runBenchmark(int);
void mergeSort(int*, int, int, int*);
void mergeKSortedArrays(int**, int*, int, int*, int);
int* newRandArray(int);


int main() {
    srand(time(NULL));

    runBenchmark(100);
    runBenchmark(1000);
    runBenchmark(10000);
    runBenchmark(100000);
    runBenchmark(1000000);
    runBenchmark(10000000);

    return 0;
}


///////////////////////////////////////////////////////////////////


/**
 * @brief Verilen boyuttaki bir dizi için k değerine göre merge sort benchmarking'i yapar
 * @param N dizinin boyutu
 */
void runBenchmark(int N) {
    int i, j, k;
    FILE* file = fopen("benchmark.txt", "a");
    if (file == NULL) exit(78);
    
    const int HOW_MANY = 12; // PDF'te "en az 10 kez" diyordu diye 12 seçtim 

    // HOW_MANY kadar rand dizi, her k değeri için aynıları kullanılmalı
    int** arrays = (int**) malloc(HOW_MANY * sizeof(int*));
    for(i = 0; i < HOW_MANY; i++) 
        arrays[i] = newRandArray(N);

    // Her k = 2..10 için aynı değerleri dizilerle test yap
    for(k = 2; k <= 10; k++) {
        double totalTime = 0;
        for(i = 0; i < HOW_MANY; i++) {
            int* sorted = (int*) malloc(N * sizeof(int));
            
            // merge sort'u çalıştır
            clock_t begin = clock();
            mergeSort(arrays[i], N, k, sorted);
            clock_t end = clock();

            // cidden sıraladı mı kontrolü (zamanlamaya dahil değil)
            for(j = 0; j < N; j++) {
                if(sorted[j] != j+1) {
                    printf("MERGESORT YANLIS CALISTI!!!!");
                    exit(78);
                }
            }

            // bu seferki zamanı ekle
            totalTime += (double)(end - begin); 
            free(sorted);
        }
        double average = totalTime / HOW_MANY;
        // sonucu dosyaya kaydet
        fprintf(file, "=> N: %d, k: %d, avgtime: %f\n", N, k, average);
    }
    
    fprintf(file, "\n");
    fclose(file);
    
    for(i = 0; i < HOW_MANY; i++) 
        free(arrays[i]);
    free(arrays);
}


/**
 * @brief k-way merging yaparak çalışan bir merge sort fonksiyonu
 * @param arr sırasız dizi
 * @param size dizinin boyutu
 * @param k bölünecek parça sayısı, k-way'deki k sayısı
 * @param result dizinin sorted sonucu
 */
void mergeSort(int* arr, int size, int k, int* result) {
    if (size == 1) result[0] = arr[0];
    if (size <= 1) return;

    int** subarrs = (int**) malloc(k * sizeof(int*));
    int* sizes = (int*) malloc(k * sizeof(int));
    int i, j;
    
    // (size + k - 1) yazarak tam bölünmeyen dizilerde kalan elemanları eşit şekilde dağıtıyorum
    int mid = (size + k - 1) / k;

    for (i = 0; i < k; i++) {
        int start = i * mid;
        int end = (start + mid < size) ? start + mid : size;
        sizes[i] = end - start;

        subarrs[i] = (int*) malloc(sizes[i] * sizeof(int));
        for (j = 0; j < sizes[i]; j++) {
            subarrs[i][j] = arr[start + j];
        }

        // rekürsif şekilde merge sort devam ediyor
        mergeSort(subarrs[i], sizes[i], k, subarrs[i]);
    }

    // k tane subarray'i birleştiriyorum (k-way merging)
    mergeKSortedArrays(subarrs, sizes, k, result, size);

    for (i = 0; i < k; i++)
        free(subarrs[i]);
    free(subarrs);

    free(sizes);
}


/**
 * @brief K adet sıralı diziyi tek bir sıralı dizi yapar
 * @param subarrs Sıralı alt diziler
 * @param sizes Her alt dizinin boyutları
 * @param k parça sayısı
 * @param result sıralanmış sonuç olarak dizi
 * @param totalSize toplam terim sayısı
 */
void mergeKSortedArrays(int** subarrs, int* sizes, int k, int* result, int totalSize) {
    // her subarr'de sorting için mevcut ilerlemeyi kaydet
    int* pointers = (int*) calloc(k, sizeof(int));
    int i, index = 0;

    while (index < totalSize) {
        int minVal = INT_MAX;
        int minIndex = -1;

        // subarrs kullanarak her subarr'nin sıralı olarak min elemanını bul
        for (i = 0; i < k; i++) {
            if (pointers[i] < sizes[i] && subarrs[i][pointers[i]] < minVal) {
                minVal = subarrs[i][pointers[i]];
                minIndex = i;
            }
        }

        if (minIndex == -1) {
            free(pointers);
            return; // fonksiyondan dön işlem bitti
        }
        
        result[index++] = minVal;
        pointers[minIndex]++;
    }

    free(pointers);
}


/**
 * @brief Verilen boyutta 1'den N'ye dek sayıları rastgele içeren bir dizi döner
 * @param N dizinin boyutu
 * @return int* rastgele [1, N] terimlerini içeren bir dizi
 */
int* newRandArray(int N) {
    int i;
    int* arr = (int*) malloc(N * sizeof(int));
    if (arr == NULL) exit(78);

    for (i = 0; i < N; i++)
        arr[i] = i + 1;  // diziyi 1-N arası sayılarla doldur

    // Fisher-Yates algoritmasıyla diziyi shuffle et (karıştır)
    for (i = N - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    return arr;
}



