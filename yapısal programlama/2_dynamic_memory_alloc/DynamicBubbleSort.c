

#include <stdlib.h>
#include <stdio.h>



int getPosNumber(const char* text);

void printMatrix(const char* text, int** mat, int row, int col);
int** getMatrix(int row, int col);
void freeMatrix(int** mat, int row);

void printArray(const char* text, int* arr, int n);
int* getArray(int n);
void freeArray(int* arr);

void bubbleSort(int** arr, int n);




int main() {
	int i;
	int n = getPosNumber("Enter a size: ");
	int* arr = getArray(n);	
	
	printArray("Your array:\n", arr, n);
	bubbleSort(&arr, n);
	printArray("Your sorted array:\n", arr, n);
	freeArray(arr);
	
	fflush(stdin);
	int row = getPosNumber("Enter row: ");
	fflush(stdin);
	int col = getPosNumber("Enter col: ");
	
	int** mat = getMatrix(row, col);
	printMatrix("Your matrix: \n", mat, row, col);
	for(i = 0; i < row; i++) {
		bubbleSort(&mat[i], col);
	}
	printMatrix("Your row-sorted matrix: \n", mat, row, col);
	freeMatrix(mat, row);
	
	return 0;
}






void freeArray(int* arr) {
	free(arr);
}



int** getMatrix(int row, int col){
	int i, j;
	int** mat = (int**) malloc(sizeof(int*) * row);
	for(i = 0; i < row; i++){
		mat[i] = (int*) malloc(sizeof(int) * col);
	}
	
	for(i = 0; i < row; i++){
		for(j = 0; j < col; j++){
			printf("Enter mat[%d][%d]: ", i, j);
			scanf("%d", &mat[i][j]);
		}
	}
	printf("\n");
	return mat;
}



void printArray(const char* text, int* arr, int n){
	printf(text);
	int i;
	for(i = 0; i < n; i++){
		printf("%3d ", arr[i]);
	}
	printf("\n");
}



int* getArray(int n){
	int* arr = (int*) malloc(n * sizeof(int));
	
	int i;
	for(i = 0; i < n; i++){
		printf("Enter arr[%d]: ", i);
		scanf("%d", &arr[i]);
	}
	printf("\n");
	
	return arr;
}



void printMatrix(const char* text, int** mat, int row, int col){
	printf(text);
	int i, j;
	for(i = 0; i < row; i++){
		for(j = 0; j < col; j++){
			printf("%3d ", mat[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}



void bubbleSort(int** arr, int n){
	int sorted = 0, i, x;
	while(sorted == 0){
		sorted = 1;
		for(i = 0; i < n-1; i++){
			if((*arr)[i] > (*arr)[i+1]){
				sorted = 0;
				x = (*arr)[i+1];
				(*arr)[i+1] = (*arr)[i];
				(*arr)[i] = x;
			}
		}
	}
}



int getPosNumber(const char* text){
	int n;
	while(n <= 0){
		printf(text);
		scanf("%d", &n);
	}
	printf("\n");
	return n;
}



void freeMatrix(int** mat, int row){
	int i;
	for(i = 0; i < row; i++){
		free(mat[i]);
	}
	free(mat);
}






