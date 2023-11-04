
/*
Oguzhan Topaloglu, C19052025 (CAP ogrencisiyim)
Yapisal Prog. Giris Projesi - Comb Sort
*/


#include <stdio.h>
#include <time.h>
#include <stdlib.h>


// kod kac farkli boyut icin calisacak onu tanimlar
// ilk boyut 10000 olarak ayarlanmistir, her iterasyonda +10000 artacaktir
#define ITER_COUNT 3



// iki degiskenin degerlerini degistirir
void swap(int* a, int* b){
	int temp = *a;
	*a = *b;
	*b = temp;
}


// comp sort algoritmasi
void combSort(int N, int arr[N]){
	int i, gap = N, swapped = 1;
  
    while (gap != 1 || swapped == 1) {
		gap /= 1.3;
		
		if(gap < 1)
			gap = 1;  
			
        swapped = 0;
  
        for (i = 0; i < N - gap; i++) {  
            if (arr[i] > arr[i + gap]) {  
            	swap(&arr[i], &arr[i + gap]);
                swapped = 1;  
            }
        }
    }  
}




// bubble sort algoritmasi, comb sort'ta gap = 1 yapilmasi ve gap'in degistirilmemesi ile ayni sey
void bubbleSort(int N, int arr[N]){
	int i, swapped = 1;  
  
    while (swapped == 1) {
        swapped = 0;
  
        for (i = 0; i < N - 1; i++) {  
            if (arr[i] > arr[i + 1]) {  
            	swap(&arr[i], &arr[i + 1]);
                swapped = 1;  
            }
        }
    }  
}




// iki tane diziyi rastgele degerlerle doldurur, iki dizi birbirinin kopyasi olacaktir
void fillArrays(int N, int arr1[N], int arr2[N]){
	srand(time(NULL));
	int i;
	int randomValue;
	for(i = 0; i < N; i++){
		randomValue = rand();
		arr1[i] = randomValue;
		arr2[i] = randomValue;
	}
}



void compareCombAndBubbleSortForN(int N, int index, int combSortTimes[ITER_COUNT], int bubbleSortTimes[ITER_COUNT]){
	int combSortTime, bubbleSortTime;
	
	// hem bubble sort hem de comb sort icin birbirinin kopyasi iki dizi
	int combSortArray[N], bubbleSortArray[N];
	
	// dizilerin rastgele terimlerle doldurulmasi
	fillArrays(N, combSortArray, bubbleSortArray);
	
	// comb sort
    clock_t t1;
    t1 = clock();
	combSort(N, combSortArray);
    t1 = clock() - t1;
    combSortTime = (int) t1;
	
	// bubble sort
    clock_t t2;
    t2 = clock();
	bubbleSort(N, bubbleSortArray);
    t2 = clock() - t2;
    bubbleSortTime = (int) t2;
	
	// zamanlarin yazdirilmasi
	printf("\nN degeri: %d", N);
	printf("\ncombSortTime: %d milisaniye", combSortTime);
	printf("\nbubbleSortTime: %d milisaniye\n", bubbleSortTime);
	
	combSortTimes[index] = combSortTime;
	bubbleSortTimes[index] = bubbleSortTime;
}



// utility fonksiyonu, konsola * yazdirmak icin
void printStars(int starCount){
	int j;
	for(j = 0; j < starCount; j++)
		printf("*");
}



// konsola yazisal ve simgesel olarak calisma surelerini yazdirir
void visualizeTimes(int combSortTimes[ITER_COUNT], int bubbleSortTimes[ITER_COUNT]){
	int i;
	
	printf("\n\n");
	
	// yazisal
	printf("\nComb sort times: \n");
	for(i = 0; i < ITER_COUNT; i++) {
		printf("%d\n", combSortTimes[i]);
	}
	printf("\nBubble sort times: \n");
	for(i = 0; i < ITER_COUNT; i++) {
		printf("%d\n", bubbleSortTimes[i]);
	}
	
	// simgesel
	printf("\nComb sort: \n");
	for(i = 0; i < ITER_COUNT; i++) {
		if(i == 0) // ilk calisma suresi 1 yildiz olsun
			printStars(1); 
		else // obur calisma surelerinin yildizlerini ilkine gore oranlayarak koy
			printStars(combSortTimes[i] / combSortTimes[0]);
		printf("\n");
	}
	printf("\nBubble sort: \n");
	for(i = 0; i < ITER_COUNT; i++) {
		if(i == 0) // ilk calisma suresi 1 yildiz olsun
			printStars(1);
		else // obur calisma surelerinin yildizlerini ilkine gore oranlayarak koy
			printStars(bubbleSortTimes[i] / bubbleSortTimes[0]);
		printf("\n");
	}
}



int main(){
	// zamanlari saklamak icin
	int combSortTimes[ITER_COUNT];
	int bubbleSortTimes[ITER_COUNT];
	
	int i;
	for(i = 0; i < ITER_COUNT; i++){
		compareCombAndBubbleSortForN((i+1) * 10000, i, combSortTimes, bubbleSortTimes);
	}
	
	visualizeTimes(combSortTimes, bubbleSortTimes);
	
	return 0;
}




