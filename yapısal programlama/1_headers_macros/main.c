#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "calculation.h"
#include "yp1.h"


/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	int x, y;
	int selection;
	int run=1;
	int result;
	int i; 
	
	while(run){
		MENU;
		scanf("%d", &selection);
		switch(selection){
			case 1:
				printf("Toplanacak ilk sayi:");
				scanf("%d", &x);
				printf("Toplanacak ikinci sayi:");
				scanf("%d", &y);
				printf("%d + %d = %d\n", x, y, topla(x,y));
				break;
			case 2:
				printf("Eksilen sayiyi giriniz:");
				scanf("%d",&x);
				printf("Cikan sayiyi giriniz:");
				scanf("%d",&y);
				printf("%d - %d = %d\n", x, y, cikar(x, y));
				break;
			case 3:
				printf("Ilk carpani giriniz:");
				scanf("%d", &x);
				printf("Ikinci carpani giriniz:");
				scanf("%d",&y);
				printf("%d * %d = %d\n ", x , y, carp(x, y));
				break;
			case 4:
				printf("Bolunen sayiyi giriniz: ");
				scanf("%d", &x);
				printf("Bolen sayiyi giriniz:");
				scanf("%d", &y);
				printf("%d / %d = %d\n", x, y, bol(x, y));
				break;
			
			case 5:
				printf("Faktoriyeli alinacak sayiyi giriniz:");
				scanf("%d",&x);
				faktoriyel(x);
				//printf("%d ! =  %d \n ",x,faktoriyel(x));
			
			case 6:
				run = 0;
				break;
			default:
				printf("Geçerli bir giriþ yapmadiniz, tekrar deneyin...\n");
				break;
			
			
		}
	}
	
	return 0;
}
