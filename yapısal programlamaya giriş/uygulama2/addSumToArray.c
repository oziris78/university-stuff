#include <stdio.h>
#define SIZE 200


/*
sirali bir dizi icindeki sayilarin ikiser ikiser toplamlarini hesaplayip dizinin sirasini bozmayarak
diziye ekleyen kodu yaz
*/
int main(){
	int arr[SIZE], n, sum, i, j, k;
	printf("Enter size: ");
	scanf("%d", &n);
	
	for(i = 0; i < n; i++){
		printf("arr[%d]: ", i);
		scanf("%d", &arr[i]);
	}	
	
	printf("Before: \n");
	for(i = 0; i < n; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");
	
	// insert
	i = 0;
	j = 2;
    while(i < n){
    	sum = arr[i] + arr[i+1];
    	
    	// sum'in konulacagi indeksi kontrol ederek j'de depola
		while(arr[j] <= sum && j < n)
    		j++;
    	
    	// tasinmasi gereken kismi tasi
		for(k = n - 1; k >= j; k--)
    		arr[k+1] = arr[k];
    	
    	// j'ye sum'i ata
    	arr[j] = sum;
    	
    	i += 2;
    	j = i + 2;
    	n++;
	}
	n--; // en son adimda yine arttirilacagindan n-- yapmaliyiz
	
	
	printf("After: \n");
	for(i = 0; i < n; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");
	
	
	return 0;
}

