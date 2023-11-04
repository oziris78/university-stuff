
#include <stdio.h>
#define SIZE 300


int main(){
	
	int arr[SIZE], n, i;
	
	// get the size of the array
	printf("Size: ");
	scanf("%d", &n);
	
	// fill the array
	for(i = 0; i < n; i++){
		printf("Enter array[%d]: ", i);
		scanf("%d", &arr[i]);
	}

	// print the array
	printf("\nYour array: ");
	for(i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");
	
	// print the result
	for(i = 0; i < n / 2; i++){
		if(arr[i] != arr[n-i-1]){
			printf("This array is NOT palindrome");
			goto END;
		}
	}
	printf("This array IS palindrome");
		
END:
		
	return 0;
}

