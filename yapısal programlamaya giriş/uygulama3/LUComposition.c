
#include <stdio.h>
#define n 3



void swap(int* a, int* b){
	int temp = *a;
	*a = *b;
	*b = temp;
}



void printVector(float arr[n]){
	int i;
	for(i=0; i<n; i++){
		printf("%f ", arr[i]);
	}
	printf("\n");	
}



void printMatrix(int row, int col, float arr[row][col]){
	int i, j;
	printf("\n");
	for(i = 0; i < row; i++){
		for(j = 0; j < col; j++){
			printf("%7.2f ", arr[i][j]);
		}
		printf("\n");
	}
}



void luDecomposition(float mat[n][n], float lower[n][n], float upper[n][n]){
	int i, j, k;
	int sum;
	
	for(i=0;i<n;i++){
		for(k=i; k<n; k++){
			sum=0;
			for(j=0; j<i; j++){
				sum += lower[i][j] * upper[j][k];
			}
			upper[i][k] = mat[i][k] - sum;			
		}
		
		for(k=i; k<n; k++){
			if(i == k)
				lower[i][k] = 1;
			else{
				sum=0;
				for(j=0; j<i; j++) {
					sum += lower[k][j] * upper[j][i];
				}
				lower[k][i] = (mat[k][i] - sum) / upper[i][i];
			}
		}
	}
}



void dot(int colLength, int rowLength, float mOne[rowLength][colLength], float mTwo[rowLength][colLength], float Res[rowLength][colLength]){
	int i,j,k;
	
	for(i=0;i<rowLength;i++){
		for(j=0;j<colLength;j++){
			float tmpSum=0;
			for(k=0;k<rowLength;k++){
				tmpSum+=mOne[i][k]*mTwo[k][j];
			}
			Res[i][j]=tmpSum;
		}
	}
}



void forwardSub(float A[n][n], float B[n], float res[n]){
	int i,j,k;
	float sum;
	
	for(i=0; i<n; i++){
		sum=0;
		for(j=0; j<i; j++){
			sum += A[i][j] * B[j];
		}
		B[i]=1.0*(res[i]-sum)/A[i][i];
	}	
}



void backwardSub(float A[n][n], float B[n], float res[n]){
	int i, j, k;
	float sum;
	
	for(i=n-1; i>=0; i--){
		sum = 0;
		for(j = i + 1; j<n; j++){
			sum += A[i][j] * B[j];
		}
		B[i] = 1.0 * (res[i]-sum) / A[i][i];
	}
}



void inverseMatrix(float A[n][n], float Inv[n][n]){
	float I[n]={0};
	float X[n]={0};
	float Z[n]={0};
	float lower[n][n];
	float upper[n][n];
	int i, j, k;
	luDecomposition(A, lower, upper);
	
	for(i=0; i<n; i++){
		I[i]=1.0;
		forwardSub(lower, Z, I);
		backwardSub(upper, X, Z);
		for(j=0; j<n; j++){
			Inv[j][i] = X[j];
		}
		I[i] = 0;
	}
}



int main(){
	float mat[n][n] = {
		{2, -1, -2},
		{-4, 6, 3},
		{-4, -2, 8}
	};
	float lower[n][n] = {0};
	float upper[n][n] = {0};
	
	float res[n][n];
	float inv[n][n]={0};
	
	
	printf("\nA:\n");
	printMatrix(n, n, mat);
	printf("\nL:\n");
	luDecomposition(mat, lower, upper);
	printMatrix(n, n, lower);
	printf("\nU:\n");
	printMatrix(n, n, upper);
	dot(n, n, lower, upper, res);
	printf("\nLU:\n");
	printMatrix(n, n, res);
	printf("\nA^-1:\n");
	inverseMatrix(mat, inv);
	printMatrix(n, n, inv);
	dot(n, n, mat, inv, res);
	printf("\nA * A^-1:\n");
	printMatrix(n, n, res);
	
	return 0;
}

