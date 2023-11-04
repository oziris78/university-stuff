

#include <stdlib.h>
#include <stdio.h>


int** allocateMatrix1(int, int);
void allocateMatrix2(int***, int, int);

int** addMatrices1(int**, int**, int, int);
void addMatrices2(int**, int**, int***, int, int);

void fillMatrix(int**, int, int);
void printMatrix(int**, int, int);
void freeMatrix(int**, int);



int main() {
	srand(time(NULL));
	int **mat1, **mat2, **mat3, **mat4;
	int n, m;
	
	n = 3 + rand() % 3; // [3,5]
	m = 3 + rand() % 3; // [3,5]
	
	mat1 = allocateMatrix1(n, m);
	allocateMatrix2(&mat2, n, m);
	
	fillMatrix(mat1, n, m);
	fillMatrix(mat2, n, m);
	
	mat3 = addMatrices1(mat1, mat2, n, m);
	addMatrices2(mat1, mat2, &mat4, n, m);
	
	printMatrix(mat1, n, m);
	printMatrix(mat2, n, m);
	printMatrix(mat3, n, m);
	printMatrix(mat4, n, m);
	
	freeMatrix(mat1, n);
	freeMatrix(mat2, n);
	freeMatrix(mat3, n);
	freeMatrix(mat4, n);
	
	return 0;
}



int** allocateMatrix1(int n, int m) {
	int** matrix;
	matrix = (int**) malloc(sizeof(int*) * n);
	int i;
	for(i = 0; i < n; i++){
		matrix[i] = (int*) malloc(sizeof(int) * m);
	}
	return matrix;
}


void allocateMatrix2(int*** matrix, int n, int m) {
	*matrix = (int**) malloc(sizeof(int*) * n);
	int i;
	for(i = 0; i < n; i++){
		(*matrix)[i] = (int*) malloc(sizeof(int) * m);
	}
}


void fillMatrix(int** mat, int n, int m) {
	int i, j;
	for(i = 0; i < n; i++){
		for(j = 0; j < m; j++){
			mat[i][j] = rand() % 10;
		}
	}
}


int** addMatrices1(int** mat1, int** mat2, int n, int m) {
	int** newMatrix = allocateMatrix1(n, m);
	int i, j;
	for(i = 0; i < n; i++){
		for(j = 0; j < m; j++){
			newMatrix[i][j] = mat1[i][j] + mat2[i][j];
		}
	}
	return newMatrix;
}


void addMatrices2(int** mat1, int** mat2, int*** newMatrix, int n, int m) {
	*newMatrix = allocateMatrix1(n, m);
	int i, j;
	for(i = 0; i < n; i++){
		for(j = 0; j < m; j++){
			(*newMatrix)[i][j] = mat1[i][j] + mat2[i][j];
		}
	}
}


void printMatrix(int** mat, int n, int m) {
	int i, j;
	for(i = 0; i < n; i++){
		for(j = 0; j < m; j++){
			printf("%4d", mat[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


void freeMatrix(int** matrix, int n){
	int i;
	for(i = 0; i < n; i++){
		free(matrix[i]);
	}
	free(matrix);
}




