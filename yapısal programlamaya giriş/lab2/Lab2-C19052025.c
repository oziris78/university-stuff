
#include <stdio.h>
#define SIZE 300

// Oguzhan Topaloglu - C19052025 (Cap ogrencisiyim)

// her zaman saga gitmeyi oncelikli goruyor ve (0,0)da basliyor
// (0,0) noktasinin 0 veya 1 olmasini umursamiyor

int main(){
	
	int arr[SIZE][SIZE], rowSize, colSize;
	int i, j;
	int route[SIZE], routeLength;
	int curX, curY;
	
	// dizi boyutlarini elde et
	do{
		printf("Dizi boyutunuz (row, col): ");
		scanf("%d %d", &rowSize, &colSize);
	}
	while(rowSize <= 0 || colSize <= 0);
		
	// diziyi doldur
	for(i = 0; i < rowSize; i++){
		for(j = 0; j < colSize; j++){
			printf("array[%d][%d]: ", i + 1, j + 1);
			scanf("%d", &arr[i][j]);
		}
	}

	// ilk basta rota var mi diye bak ve eger yoksa programi sonlandir
	if(arr[0][1] == 0 && arr[1][0] == 0){
		printf("\n\nBu matriste herhangi bir rota bulunmamaktadir.\n\n");
		return 0;
	}
	
	// rota hesapla (buraya geldiyse demek ki rota var)
	routeLength = 0;
	curX = 0;
	curY = 0;
	while(0 == 0){ // while(true) yani sonsuz dongu
		if(arr[curX][curY+1] == 1 && curY < colSize){ // saga gidebiliyorsa gitsin
			curY++;
			route[routeLength++] = 1; // saga gittik diye 1
		}
		else if(arr[curX+1][curY] == 1 && curX < rowSize){ // asagiya gidebiliyorsa gitsin
			curX++;
			route[routeLength++] = 2; // asagiya gittik diye 2
		}
		else{
			break;
		}
	}
	
	// sonucu ekrana yaz
	printf("\nRota: ");
	for(i = 0; i < routeLength; i++) {
		printf("%d ", route[i]);
	}
	printf("\n");
	
	
	return 0;
}



