
#include <stdio.h>
#include <limits.h>
#define SIZE 100


int main(){

	int arr[SIZE][SIZE], K[SIZE] = {0}, rowSize, colSize, i, j;
	int max = INT_MIN;
	
	// get dimensions
	do{
		printf("Dizi boyutunuz (row, col): ");
		scanf("%d %d", &rowSize, &colSize);
	}
	while(rowSize <= 0 || colSize <= 0);
		
	// get array while getting the max
	for(i = 0; i < rowSize; i++){
		for(j = 0; j < colSize; j++){
			printf("array[%d][%d]: ", i+1, j+1);
			scanf("%d", &arr[i][j]);
			if(arr[i][j] > max){
				max = arr[i][j];
			}
			K[arr[i][j]]++;
		}
	}
	
	printf("\nUnique values: ");
	for(i = 0; i <= max; i++){
		if(K[i] == 1){
			printf("%d ", i);
		}
	}
	
		
	return 0;
}

