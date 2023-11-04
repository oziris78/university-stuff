

#include <stdlib.h>
#include <stdio.h>

// bazen calismiyor neden anlamadim ve anlamaya calisamayacak kadar uykum var

typedef struct term {
	int coef;
	int degree;
} TERM; // a x^n


typedef struct polynomial {
	TERM* terms;
	int numberOfTerms;
} POLY;


POLY* getRandomPoly();
void printPoly(POLY*);
POLY* addPoly(POLY*, POLY*);


int main() {
	srand(time(NULL));
	POLY *p1, *p2, *p3;
	
	p1 = getRandomPoly();
	p2 = getRandomPoly();
	p3 = addPoly(p1, p2);
	
	printf("P(X): \n");  
	printPoly(p1);
	
	printf("Q(X): \n");  
	printPoly(p2);
	printf("P(X) + Q(X): \n");  
	printPoly(p3);
	
	return 0;
}



POLY* addPoly(POLY* p1, POLY* p2) {
	int maxDegree = 1;
	int i, temp;
	for(i = 0; i < p1->numberOfTerms; i++){
		temp = p1->terms[i].degree;
		if(temp > maxDegree) maxDegree = temp;
	}
	for(i = 0; i < p2->numberOfTerms; i++){
		temp = p2->terms[i].degree;
		if(temp > maxDegree) maxDegree = temp;
	}
	int* coefs = (int*) calloc(maxDegree, sizeof(int));
	int deg, coef, count;
	for(i = 0; i < p1->numberOfTerms; i++){
		coef = p1->terms[i].coef;
		deg = p1->terms[i].degree;
		coefs[deg] += coef;
	}
	for(i = 0; i < p2->numberOfTerms; i++){
		coef = p2->terms[i].coef;
		deg = p2->terms[i].degree;
		coefs[deg] += coef;
	}
	for(i = 0; i <= maxDegree; i++){
		temp = coefs[i];
		if(temp != 0){
			count++;
		}
	}
	POLY* newPoly = (POLY*) malloc(sizeof(POLY));
	newPoly->numberOfTerms = count;
	int j = 0;
	newPoly->terms = (TERM*) malloc(newPoly->numberOfTerms * sizeof(TERM));
	for(i = 0; i <= maxDegree; i++){
		if(coefs[i] != 0){
			newPoly->terms[j].coef = coefs[i];
			newPoly->terms[j].degree = i;
			j++;
		}
	}
	
	return newPoly;
}



POLY* getRandomPoly() {
	POLY* poly = (POLY*) malloc(sizeof(POLY));
	poly->numberOfTerms = 3 + rand() % 10;
	poly->terms = (TERM*) malloc(poly->numberOfTerms * sizeof(TERM));
	int taken[100] = {0};
	int i, deg;
	for(i = 0; i < poly->numberOfTerms; i++){
		poly->terms[i].coef = 3 + rand() % 9;
		while(1){
			deg = 1 + rand() % 9;
			if(taken[deg] == 0){
				taken[deg] = 1;
				break;
			}		
		}
		poly->terms[i].degree = deg;
	}
	return poly;
}


void printPoly(POLY* poly) {
	int i;
	for(i = 0; i < poly->numberOfTerms; i++){
		printf("%d x^%d", poly->terms[i].coef, poly->terms[i].degree);
		if(i+1 != poly->numberOfTerms)
			printf(" + ");
	}
	printf("\n\n");
}



