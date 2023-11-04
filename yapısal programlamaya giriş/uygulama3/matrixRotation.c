#include <stdio.h>
#define R 4
#define C 4


void swap(int* a, int* b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

void reverseRows(int arr[R][C]){
	int i,j,k;
	for(j=0; j<C; j++){
		for(i=0,k=R-1; i<k; i++,k--) {
			swap(&arr[i][j], &arr[k][j]);
		}
	}
}

void reverseColumns(int arr[R][C]){
	int i,j,k;
	for(i=0; i<R; i++){
		for(j=0,k=C-1; j<k; j++,k--){
			swap(&arr[i][j], &arr[i][k]);
		}
	}
}

void transpose(int arr[R][C]){
	int i, j, temp;
	for(i = 0; i < R; i++){
		for(j = i; j < C; j++){
			swap(&arr[i][j], &arr[j][i]);
		}
	}
}

void rotate90(int arr[R][C]){
	transpose(arr);
	reverseColumns(arr);
}

void rotateAnti90(int arr[R][C]){
	transpose(arr);
	reverseRows(arr);
}

void printMatrix(int arr[R][C]){
	int i, j;
	printf("\nYour matrix: \n");
	for(i = 0; i < R; i++){
		for(j = 0; j < C; j++){
			printf("%d ", arr[i][j]);
		}
		printf("\n");
	}
}


int main(){
	int arr[R][C] = {
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 10, 11, 12},
		{13, 14, 15, 16}
	};
	
	printMatrix(arr);
	printf("\n");
	rotateAnti90(arr);
	printMatrix(arr);
	


	
	return 0;
}

