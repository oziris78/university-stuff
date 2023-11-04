
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 3


// Oguzhan Topaloglu - C19052025 (Cap ogrencisiyim)


// parametre olarak alinan matrisi rakamlarla [0,9] doldurur
void fillMatrixWithRandomValues(int mat[N][N]) {
	int i, j;
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			mat[i][j] = rand() % 10;
		}
	}
}


// recursive fonksiyon
void calculateMinors(int mat[N][N], int minor[N][N], int row, int col){
	minor[row][col] = det(mat, row, col);
	col++;
	if(col == 3){
		col = 0;
		row++;
	}
	if(row == 3 && col == 0) // eger son cell de doldurulduysa artik don
		return;
		
	calculateMinors(mat, minor, row, col);
}


// mat matrisinden row ve col satir sutunlarini cikarip det hesaplar kalan kisimda
int det(int mat[N][N], int row, int col){
	// inner matrisini bulma kismi kendi fonksiyonuna konulabilir ya da dogrudan
	// calculateMinors fonksiyonu icine konabilir, determinantla alakasi olmadigi halde
	// burada hesaplaniyor ancak duzenli oldu diye dokunmuyorum
	int inner[N-1][N-1];
	int i, j;
	int innerX = 0, innerY = 0;
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			if(i == row || j == col)
				continue;
			inner[innerX][innerY] = mat[i][j];
			innerY++;
			if(innerY == N-1){
				innerY = 0;
				innerX++;
			}
		}
	}
	
	return inner[0][0] * inner[1][1] - inner[0][1] * inner[1][0];
}


void printMatrix(int R, int C, int mat[R][C]){
	int i, j;
	for(i = 0; i < R; i++){
		for(j = 0; j < C; j++){
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}
}


int main(){
	srand(time(NULL));
	
	int mat[N][N];
	int minor[N][N];
	
	printf("Rastgele matris: \n");
	fillMatrixWithRandomValues(mat);
	printMatrix(N, N, mat);
	
	printf("\nMinorler: \n");
	calculateMinors(mat, minor, 0, 0);
	printMatrix(N, N, minor);
	
	return 0;
}



