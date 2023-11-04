

#include <stdlib.h>
#include <stdio.h>

void f(int** arr, int n);

int main() {
	int* arr;
	
	f(&arr, 5);
	
	return 0;
}


void f(int** arr, int n){
	*arr = (int*) malloc(n * sizeof(int));
	
	int i;
	for(i = 0; i < n; i++){
		(*arr)[i] = 2*i;
	}
	
	for(i = 0; i < n; i++){
		printf("%3d ", (*arr)[i]);
	}
}




