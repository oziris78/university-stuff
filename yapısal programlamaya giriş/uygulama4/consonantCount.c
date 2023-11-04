
#include <stdio.h>
#include <string.h>
#include <ctype.h>


int isConsonant(char ch){
	ch = toupper(ch);
	
	return !(ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U') && ch >= 65 && ch <= 90;
}


// iterative version
int totalConsonantCount(char* str){
	int count = 0;
	int i;
	for(i = 0; i < strlen(str); i++){
		if(isConsonant(str[i]))
			count++;
	}
	return count;
}


// recursive versions
int totalConsonantCountRecursive(char* str, int n){
	if(str[n] == '\0')
		return 0;
		
	return totalConsonantCountRecursive(str, n+1) + isConsonant(str[n]);
}

int totalConsonantCountRecursiveReverse(char* str, int n){
	if(n == 1)
		return isConsonant(str[0]);
		
	return totalConsonantCountRecursive(str, n-1) + isConsonant(str[n-1]);
}





int main(){
	char str[100] = "abceaeaeaea effffff f f";
	
	printf("%d\n", totalConsonantCount(str));
	
	printf("%d\n", totalConsonantCountRecursive(str, 0));
	
	printf("%d\n", totalConsonantCountRecursiveReverse(str, 0));
	
	
	return 0;
}


