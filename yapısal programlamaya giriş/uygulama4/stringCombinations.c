
#include <stdio.h>
#include <string.h>
#include <ctype.h>



void strConcat(char* dest, char source){
	int firstLen = strlen(dest);
	dest[firstLen] = source;
	dest[firstLen + 1] = '\0';
}



void printAllKLengthrec(char set[], char *prefix, int n, int k){
	int i;
	char newPrefix[100];
	if(k==0){
		printf("%s \n", prefix);
		return;
	}
	
	for(i=0;i<n;i++){
		strcpy(newPrefix, prefix);
		strConcat(newPrefix, set[i]);
		printAllKLengthrec(set, newPrefix, n, k-1);
	}
	
	
}



void printAllKLength(char set[],int k,int n){
	char prefix[100]={""};
	printAllKLengthrec(set, prefix, n, k);
	
}



int main(){
	char set1[] = {'a','b'};
	int k = 3;
	printAllKLength(set1, k, 2);
}

