
#include <stdio.h>
#include <stdbool.h>
#define SIZE 100

// Oguzhan Topaloglu, C19052025 (cap ogrencisiyim)

int main(){

	int arr[SIZE], K[SIZE], n, i;
	
	// boyutu al
	do{
		printf("Dizi boyutunuz: ");
		scanf("%d", &n);
	}
	while(n <= 0);
		
	// diziyi doldur
	for(i = 0; i < n; i++){
		printf("array[%d]: ", i);
		scanf("%d", &arr[i]);
	}
	
	///////////////////////////////////////////////////////////////////////
	
	// once arr'deki max terim gerekli, O(N)
	int max = 0;
	for(i = 0; i < n; i++){
		if(arr[i] > max){
			max = arr[i];
		}
	}

	// K dizisini max terime dek dolduracagim [0, max] doldurulacak, O(max)
	for(i = 0; i <= max; i++){
		K[i] = 0;		
	}
	
	// her terim kac kez geciyorsa K[terim]++; yapilmali ki counting sort'taki gibi sayim yapilsin, O(N)
	for(i = 0; i < n; i++){
		int term = arr[i];
		// burada term kesinlikle [0, max] araliginda dogrudan arttirabilirim
		K[term]++;
	}
	
	// sihirli sayilari yazdir
	int foundMagic = -1;
	printf("Sihirli sayilariniz: ");
	// bu dongu i = 0 ile baslatilirsa 0 da katilmis olur, O(max)
	// pdf'te 0 katilmadigi icin 1'den baslattim
	for(i = 1; i <= max; i++){ 
		if(i == K[i]){
			printf("%d ", i);
			foundMagic = 1;
		}
	}
	if(foundMagic == -1){
		printf("Dizinizde hic sihirli sayi bulunmamaktadir...");
	}
	
	// 5 tane lineer dongu  =>  O(N) + O(N) + O(N) + O(max) + O(max) =  3 O(N) + 2 O(max)
	
		
	return 0;
}

