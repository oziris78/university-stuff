
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 300


int main(){
	srand(time(NULL));
	
	int arr[SIZE], n, i, j, t;
	
	// get the size of the array
	printf("Size: ");
	scanf("%d", &n);
	
	// fill the array
	for(i = 0; i < n; i++){
		printf("Enter array[%d]: ", i);
		scanf("%d", &arr[i]);
	}

	// print the array
	printf("\nYour array: ");
	for(i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");
	
	// shuffle it
    for (i = 0; i < n - 1; i++) {
    	j = rand() % n;
    	t = arr[j];
    	arr[j] = arr[i];
    	arr[i] = t;
    }
    
    // get random numbers in range
    for(i = 0; i < 10; i++) {
    	int lower = -4;
    	int upper = 10;
		printf("\nIn range [%d,%d] number is %d\n", lower, upper, (rand() % (upper - lower + 1)) + lower);
	}
	
	
	// print the array
	printf("\nYour array: ");
	for(i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");
	
	
	return 0;
}

