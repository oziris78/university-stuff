#include <stdio.h>
#define SIZE 20


int main(){
	
	int num, binary[SIZE], i = 0;
	printf("Enter number in base 10: ");
	scanf("%d", &num);
	while(num >= 1){
		binary[i] = num % 2;
		num /= 2;
		i++;
	}
	
	printf("\nNumber in base 2: ");
	for(i = i - 1; i >= 0; i--){
		printf("%d", binary[i]);
	}
	
	
	
	return 0;
}
