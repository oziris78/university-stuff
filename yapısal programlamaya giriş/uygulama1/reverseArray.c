#include <stdio.h>
#define SIZE 20

int main(){
	
	int arr[SIZE], n, i, temp;

	do{
		printf("Enter size: ");
		scanf("%d", &n);
	}while(n > SIZE);
	
	for(i = 0; i < n; i++){
		printf("arr[%d]: ", i+1);
		scanf("%d", &arr[i]);
	}
	
	printf("arr: ");
	for(i = 0; i < n; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");
	
	for(i = 0; i < n/2; i++){
		temp = arr[i];
		arr[i] = arr[n - 1 - i];
		arr[n - 1 - i] = temp;
	}
	
	printf("reversed arr: ");
	for(i = 0; i < n; i++){
		printf("%d ", arr[i]);
	}
	
	return 0;
}

