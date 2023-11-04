#include <stdio.h>
#define SIZE 200

/*
nxn bir matriste kac adet ilk kosegene gore esit deger var

1   2   3
2   3   4*
3   1*   1

bu ornekte 1 olacak
*/
int main(){
	
	int mat[SIZE][SIZE], size, i, j, counter = 0; 
	printf("enter size: ");
	scanf("%d", &size);
	
	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			printf("mat[%d][%d]: ", i, j);
			scanf("%d", &mat[i][j]);
		}
	}
	
	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			printf("%5d ", mat[i][j]);
		}
		printf("\n");
	}
	
	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			if(i != j && mat[i][j] != mat[j][i])
				counter++;
		}
	}
	
	counter /= 2;
	printf("%d", counter);
	
	return 0;
}

