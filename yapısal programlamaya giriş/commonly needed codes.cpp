



// Get array size
do{
	printf("Dizi boyutunuz: ");
	scanf("%d", &n);
}
while(n <= 0);



// Fill array
for(i = 0; i < n; i++){
	printf("Enter array[%d]: ", i);
	scanf("%d", &arr[i]);
}



// Get matrix size
do{
	printf("Dizi boyutunuz (row, col): ");
	scanf("%d %d", &rowSize, &colSize);
}
while(rowSize <= 0 || colSize <= 0);



// Fill matrix
for(i = 0; i < rowSize; i++){
	for(j = 0; j < colSize; j++){
		printf("array[%d][%d]: ", i+1, j+1);
		scanf("%d", &arr[i][j]);
	}
}



// Print Array
printf("\nYour array: ");
for(i = 0; i < n; i++) {
	printf("%d ", arr[i]);
}
printf("\n");



// Print Matrix
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

printf("\nYour matrix: \n");
for(i = 0; i < rowSize; i++){
	for(j = 0; j < colSize; j++){
		printf("%d ", arr[i][j]);
	}
	printf("\n");
}



// Random Numbers
#include <stdlib.h>
#include <time.h>
srand(time(NULL));



// Shuffle Array
for (i = 0; i < n - 1; i++) {
	j = rand() % n;
	t = arr[j];
	arr[j] = arr[i];
	arr[i] = t;
}



// Get Random Number in range [lower, upper]
for(i = 0; i < 10; i++) {
	int lower = -4;
	int upper = 10;
	int value = (rand() % (upper - lower + 1)) + lower;
	printf("\nIn range [%d,%d] number is %d\n", lower, upper, value);
}



