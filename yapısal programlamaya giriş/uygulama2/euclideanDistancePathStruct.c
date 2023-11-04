#include <stdio.h>
#include <math.h>
#include <float.h>
#define SIZE 200



typedef struct sPoint{
	int x, y;
} Point;

/*
(x,y) seklinde alinan n tane nokta icin her iki nokta arasinda en kisa yolu bulup indislerle yolu veren kodu yaz
input: (2, 3), (9, 4), (7, 5), (8, 10), (3, 6)
output: 0, 4, 2, 1, 3
*/
int main(){
	int path[SIZE], inPath[SIZE] = {0};
	Point arr[SIZE];
	int currentIndex, newIndex;
	int n, i, j;
	float distance, minDistance;
	float xDif, yDif;
	
	printf("Enter size: ");
	scanf("%d", &n);
	
	for(i = 0; i < n; i++){
		printf("(x%d,y%d): ", i+1, i+1);
		scanf("%d %d", &arr[i].x, &arr[i].y);
	}	
	
	printf("\nPoints: ");
	for(i = 0; i < n; i++){
		printf("(%d,%d)", arr[i].x, arr[i].y);
		if(i + 1 != n)
			printf(", ");
	}
	printf("\n");
	
	
	// algorithm
	path[0] = 0;
	inPath[0] = 1;
	currentIndex = 0;
	
	for(i = 1; i < n; i++){
		minDistance = FLT_MAX;
		for(j = 1; j < n; j++){
			xDif = arr[currentIndex].x - arr[j].x;
			yDif = arr[currentIndex].y - arr[j].y;
			distance = sqrt(pow(xDif, 2) + pow(yDif, 2));
			if(distance < minDistance && inPath[j] == 0){
				minDistance = distance;
				newIndex = j;
			}
		}
		path[i] = newIndex;
		inPath[newIndex] = 1;
		currentIndex = newIndex;
	}
	
	// output
	printf("\nPath: ");
	for(i = 0; i < n; i++){
		printf("%d ", path[i]);
	}
	printf("\n");
	
	return 0;
}

