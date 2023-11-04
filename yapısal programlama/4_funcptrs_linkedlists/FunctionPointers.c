

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



void sort(int* arr, int n, int(*func)(int, int));
void initArray(int** arr, int* n);
void printArray(int* arr, int n);
int asc(int x, int y);
int desc(int x, int y);


	
int main() {
	int n, *arr;
	initArray(&arr, &n);
	printArray(arr, n);
	
	printf("-----------------\n");
	sort(arr, n, desc);
	printArray(arr, n);
	sort(arr, n, asc);
	printArray(arr, n);
	
	printf("-----------------\n");
	int (*funcs[2])(int, int) = { desc, asc };
	sort(arr, n, funcs[0]);
	printArray(arr, n);
	sort(arr, n, funcs[1]);
	printArray(arr, n);
	
	printf("-----------------\n");
	int(*func)(int, int);
	func = desc;
	sort(arr, n, func);
	printArray(arr, n);
	func = asc;
	sort(arr, n, func);
	printArray(arr, n);
	
	return 0;
}



void printArray(int* arr, int n){
	int i;
	for(i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");
}



void sort(int* arr, int n, int(*func)(int, int) ){
	int i, temp, sorted = 0;
	while(sorted == 0){
		sorted = 1;
		for(i = 0; i < n-1; i++){
			if(func(arr[i], arr[i+1]) > 0){
				sorted = 0;
				temp = arr[i];
				arr[i] = arr[i+1];
				arr[i+1] = temp;
			}
		}
	}
}



int asc(int x, int y){
	return x - y;
}



int desc(int x, int y){
	return y - x;
}



void initArray(int** arr, int* n){
	printf("Enter n: ");
	scanf("%d", n);
	
	*arr = (int*) malloc(sizeof(int) * (*n));
	int i;
	for(i = 0; i < *n; i++){
		printf("Enter arr[%d]: ", i);
		scanf("%d", &((*arr)[i]) );
	}
}



