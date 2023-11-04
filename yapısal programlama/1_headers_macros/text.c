#include <stdio.h>
#include <stdlib.h>


int main(){
	char Main_Text[500]="I WAnt More Text to Search in Main Text";
	char Searched[500];
	int Main_text_length=0;
	int Searched_text_length=0;
	int i=0;
	int j;
	int N=20;
	int Control=0;
	while(Main_Text[i]!='\0'){
		Main_Text[i]=tolower(Main_Text[i]);
		i++;
	}
	Main_text_length=i;
	
	printf("Searched Text: ");
	scanf("%s",Searched);
	i=0;
	while(Searched[i]!='\0'){
		Searched[i]=tolower(Searched[i]);
		i++;
	}
	Searched_text_length=i;
	printf("Searched: %s, Length: %d, Main Length: %d\n",Searched,Searched_text_length,Main_text_length);
	for(i=0;i<Main_text_length-Searched_text_length+1;i++){
		j=0;
		while((j<Searched_text_length)&&(Searched[j]==Main_Text[i+j]))
			j++;
		if(j==Searched_text_length){
			printf("Found: %d to %d\n",i+1,i+j+1);
			Control++;
		}
	}
	
	if(Control==0){
		printf("Bulanamadi");
	}else{
		printf("Toplam Tekrar %d",Control);
	}
	
	
	
}
