#include <stdio.h>
#include <stdlib.h>
#include "yp1.h"

int topla(int x, int y){
	return (x+y);
}
int cikar(int x, int y){
	return(x-y);
}
int carp(int x, int y){
	return(x*y);
}
int bol(int x, int y){
	return(x/y);
}
int faktoriyel(int x){
	int result= 1;
	int i = x;
	while(x>1){
		result *=  x--;
	} 
	for(i; i>=0; i--)
	{					
		if(i == 1){
			printf("%d = %d \n", i, result);
		}
		else if(i>1)
		{
			printf("%d * ", i);	
		}else{
			printf("%d \n", result);
		}
	}
	return result;

	
}
