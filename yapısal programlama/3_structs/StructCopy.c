

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Data {
	int x;
	char* str;
} DATA;


int main() {
	DATA a = {123, "test"}, b;
	
	// method 1 - assigment operatoru ile kopyala
	// b = a;
	
	// method 2 - teker teker kopyala
	b.x = a.x;
	b.str = (char*) malloc(strlen(a.str) * sizeof(char));
	strcpy(b.str, a.str);
	
	
	// method 3 - memcpy fonksiyonu ile kopyala
	// memcpy(&b, &a, sizeof(a));
	
	a.x = 999;
	a.str = "999";
	
	printf("%d, %s\n", a.x, a.str);
	printf("%d, %s\n", b.x, b.str);
	
	
	return 0;
}




