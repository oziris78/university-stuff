

#include <stdlib.h>
#include <stdio.h>

void f(int*** mat, int n, int m);


int main() {
	int** mat;
	
	f(&mat, 10, 3);
	
	return 0;
}


void f(int*** mat, int row, int col){
	*mat = (int**) malloc(row * sizeof(int*));
	
	int i, j;
	for(i = 0; i < row; i++){
		(*mat)[i] = (int*) malloc(col * sizeof(int));
	}
	
	for(i = 0; i < row; i++){
		for(j = 0; j < col; j++){
			(*mat)[i][j] = i*2 + j*3;
		}
	}
	
	for(i = 0; i < row; i++){
		for(j = 0; j < col; j++){
			printf("%3d ", (*mat)[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	
}



