
#include <stdio.h>
#define SIZE 100


int main(){

	int arr[SIZE][SIZE], rowSize, colSize, i, j, val;
	
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
		}
	}
	printf("\nCumleniz: ");
	
	for(i = 0; i < rowSize; i++){
		for(j = 0; j < colSize; j++){
			val = arr[i][j];
			if(val == 0){
				break;
			}
			printf("%c", val);
		}
		printf(" ");
	}
	
		
	return 0;
}

