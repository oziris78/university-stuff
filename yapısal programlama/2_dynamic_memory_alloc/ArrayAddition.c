

#include <stdlib.h>
#include <stdio.h>


int* allocateArray1(int);
void allocateArray2(int**, int);
void fillArray(int*, int);
int* addArrays1(int*, int*, int);
void addArrays2(int*, int*, int**, int);
void printArray(int*, int);



int main() {
	srand(time(NULL));
	int *arr1, *arr2, *arr3, *arr4;
	int n;
	
	n = 5 + rand() % 6; // [5,10]
	
	arr1 = allocateArray1(n);
	allocateArray2(&arr2, n);
	
	fillArray(arr1, n);
	fillArray(arr2, n);

	arr3 = addArrays1(arr1, arr2, n);
	addArrays2(arr1, arr2, &arr4, n);
	
	printArray(arr1, n);
	printArray(arr2, n);
	printArray(arr3, n);
	printArray(arr4, n);
	
	free(arr1);
	free(arr2);
	free(arr3);
	free(arr4);
	
	return 0;
}




int* allocateArray1(int n) {
	int* arr = (int*) malloc(n * sizeof(int));
	return arr;	
}



void allocateArray2(int** arr, int n) {
	*arr = (int*) malloc(n * sizeof(int));
}



void fillArray(int* arr, int n) {
	int i;
	for(i = 0; i < n; i++) {
		arr[i] = rand() % 9;
	}
}



int* addArrays1(int* arr1, int* arr2, int n) {
	int* newArray = allocateArray1(n);
	int i;
	for(i = 0; i < n; i++) {
		newArray[i] = arr1[i] + arr2[i];
	}
	return newArray;
}



void addArrays2(int* arr1, int* arr2, int** newArray, int n) {
	*newArray = allocateArray1(n);
	int i;
	for(i = 0; i < n; i++) {
		(*newArray)[i] = arr1[i] + arr2[i];
	}
}



void printArray(int* arr, int n) {
	int i;
	for(i = 0; i < n; i++) {
		printf("%3d ", arr[i]);
	}
	printf("\n\n");
}



