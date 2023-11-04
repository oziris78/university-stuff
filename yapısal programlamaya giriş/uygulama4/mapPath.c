#include<stdio.h>
#define n 3



void printPath(int path[][2], int visitedCount){
	int i;
	for(i=0; i < visitedCount; i++){
		printf("(%d,%d) ", path[i][0], path[i][1]);
	}
	printf("\n");
}



int isVisited(int path[][2], int visitedCount, int row, int col){
	int i;
	
	for(i = 0; i < visitedCount; i++){
		if(path[i][0] == row && path[i][1] == col)
			return 0;
	}
	return 1;
}



int isValid(int path[][2], int visitedCount, int row, int col){
	return row >= 0 && row < n && col >= 0 && col < n && isVisited(path, visitedCount, row, col);
}



void addPath(int path[][2], int* visitedCount, int addingRow, int addingCol){
	path[*visitedCount][0] = addingRow;
	path[*visitedCount][1] = addingCol;
	(*visitedCount)++;
}



void removePath(int *visitedCount){
	(*visitedCount)--;
}



int findPath(int mat[n][n], int path[][2], int* visitedCount, int currentRow, int currentCol){
	int row[] = { -1, 0, 0, 1 };
	int col[] = { 0, -1, 1, 0 };
	int i;
	addPath(path, visitedCount, currentRow, currentCol);
	
	if(currentRow == n-1 && currentCol == n-1)
		return 1;
		
	int currentStep = mat[currentRow][currentCol];
	
	for(i = 0; i < 4; i++){
		int x = currentRow + row[i] * currentStep;
		int y = currentCol + col[i] * currentStep;
		
		if(isValid(path, *visitedCount, x, y) && findPath(mat, path, visitedCount, x, y))
			return 1;
	}
	
	removePath(visitedCount);
	return 0;
}




int main(){
	/*	
	int mat[n][n] = {
        { 7, 1, 3, 5, 3, 6, 1, 1, 7, 5 },
        { 2, 3, 6, 1, 1, 6, 6, 6, 1, 2 },
        { 6, 1, 7, 2, 1, 4, 7, 6, 6, 2 },
        { 6, 6, 7, 1, 3, 3, 5, 1, 3, 4 },
        { 5, 5, 6, 1, 5, 4, 6, 1, 7, 4 },
        { 3, 5, 5, 2, 7, 5, 3, 4, 3, 6 },
        { 4, 1, 4, 3, 6, 4, 5, 3, 2, 6 },
        { 4, 4, 1, 7, 4, 3, 3, 1, 4, 2 },
        { 4, 4, 5, 1, 5, 2, 3, 5, 3, 5 },
        { 3, 6, 3, 5, 2, 2, 6, 4, 2, 1 }
    };
	*/
    int mat[3][3]={
		{2, 3, 4},
		{1, 1, 1},
		{1, 1, 1}
	};
    
    int path[100][2] = {0};
    int visitedCount = 0;
    
    
    if(findPath(mat, path, &visitedCount, 0, 0)){
    	printPath(path, visitedCount);
	}
}





