
/*
 * Veri Yapilari dersi - Odev #3
 * Oguzhan Topaloglu, C19052025
 * (28-29).04.2024
*/


#include <stdlib.h>
#include <time.h>
#include <stdio.h>


// Functions that are sequentially called from main()
void readQueueSizes(int**, int*, int*);
int** getRandMatrix(int**, int, int);
void printHeapifyPrint(int**, int*, int, int);
void emptyMatrix(int**, int*, int, int);

// Helper functions
void readIntFromUser(const char*, int*, int, int);
void printOrderArray(const char*, int*, int);
void printMatrix(int**, int, int);
void maxHeapifyArray(int*, int, int);


// -------------------  MAIN FUNCTIONS  ------------------- //


/**
 * @brief Reads N and M values for a NxM matrix and the size of each queue.
 * 
 * @param qsPtr Pointer to store the array of queue sizes.
 * @param nPtr Pointer to store the value of N.
 * @param mPtr Pointer to store the value of M.
 */
void readQueueSizes(int** qsPtr, int* nPtr, int* mPtr) {
    int N, M, i;
    printf("Enter N and M values for a NxM matrix.\n");
    readIntFromUser("N value: ", &N, 1, 50); // assume user wont enter >50 number
    readIntFromUser("M value: ", &M, 1, 50); // assume user wont enter >50 number

    int* queueSizes = (int*) malloc(sizeof(int) * N);
    printf("Now enter the size of each queue.\n");
    for (i = 0; i < N; i++) {
        char prompt[1024] = {0};
        sprintf(prompt, "Size of queue #%d: ", i);
        readIntFromUser(prompt, &queueSizes[i], 1, M);        
    }
    printf("\n");

    *nPtr = N;
    *mPtr = M;
    *qsPtr = queueSizes;
}



/**
 * @brief Constructs a unique and random matrix based on given queue sizes.
 * 
 * @param queueSizesPtr Pointer to the array of queue sizes.
 * @param N The number of rows in the matrix.
 * @param M The number of columns in the matrix.
 * @return int** Pointer to the constructed matrix.
 */
int** getRandMatrix(int** queueSizesPtr, int N, int M) {
    srand(time(NULL));
    int* queueSizes = *queueSizesPtr;
    int i, j;

    int K = 2 * N * M;
    int* uniqueEnsurer = (int*) calloc(K, sizeof(int));

    int** matrix = (int**) malloc(sizeof(int*) * N);
    for (i = 0; i < N; i++) {
        matrix[i] = (int*) malloc(sizeof(int) * M);
        for (j = 0; j < M; j++) {
            matrix[i][j] = -1;

            // Find a random and unique value
            if(j < queueSizes[i]) {
                int found = 0;
                while(!found) {
                    int value = rand() % K;
                    if(uniqueEnsurer[value] != 1) {
                        found = 1;
                        uniqueEnsurer[value] = 1;
                        matrix[i][j] = value;
                    }
                }
            }
        }
    }

    free(uniqueEnsurer);
    return matrix;
}



/**
 * @brief Prints the original matrix, max-heapifies it, and prints the max-heapified version.
 * 
 * @param matrix The original matrix.
 * @param queueSizes The array of queue sizes.
 * @param N The number of rows in the matrix.
 * @param M The number of columns in the matrix.
 */
void printHeapifyPrint(int** matrix, int* queueSizes, int N, int M) {
    printf("Randomly constructed matrix: \n");
    printMatrix(matrix, N, M);

    int i, si;
    for (i = 0; i < N; i++) { // max-heapify each row
        int n = queueSizes[i];
        for(si = (n / 2) - 1; si >= 0; si--) {
            maxHeapifyArray(matrix[i], n, si);
        }
    }

    printf("Max-heapified version of the matrix: \n");
    printMatrix(matrix, N, M);
}



/**
 * @brief Empties the matrix by removing elements in a specific order (defined in the
 *              homework PDF) and prints the emptying order.
 * 
 * @param matrix The matrix to be emptied.
 * @param queueSizes The array of queue sizes.
 * @param N The number of rows in the matrix.
 * @param M The number of columns in the matrix.
 */
void emptyMatrix(int** matrix, int* queueSizes, int N, int M) {
    int i, si;
    int* emptyOrder = (int*) calloc(N, sizeof(int));
    int emptyOrderIndex = 0;
    
    while(emptyOrderIndex < N) {
        // Find the queue with the maximum value in the matrix
        int maxValue = -1, maxIndex = -1;
        for (i = 0; i < N; i++) {
            if(matrix[i][0] > maxValue) {
                maxValue = matrix[i][0];
                maxIndex = i;
            }
        }
        printf("Picked value: %d\n", matrix[maxIndex][0]);
        
        // Remove the maximum value by shifting everything to the left once
        for(i = 0; i < queueSizes[maxIndex]-1; i++) {
            matrix[maxIndex][i] = matrix[maxIndex][i+1];
        }
        matrix[maxIndex][queueSizes[maxIndex]-1] = -1;
        
        // Heapify the queue once again
        int n = queueSizes[maxIndex];
        for(si = (n / 2) - 1; si >= 0; si--) {
            maxHeapifyArray(matrix[maxIndex], n, si);
        }

        // If that row is now empty, save its index to determine the emptying order
        if(matrix[maxIndex][0] == -1) {
            emptyOrder[emptyOrderIndex++] = maxIndex;
        }

        printOrderArray("Current emptying order: ", emptyOrder, emptyOrderIndex);
        printMatrix(matrix, N, M);
    }
    
    printOrderArray("Emptying order is: ", emptyOrder, N);

    for (i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(queueSizes);
}



// -------------------  MAIN  ------------------- //


int main() {
    // Steps 1, 2 : Read N, M and size of each row
    int N, M, *queueSizes;
    readQueueSizes(&queueSizes, &N, &M);

    // Step 3: Construct the unique and random matrix
    int** matrix = getRandMatrix(&queueSizes, N, M);    

    // Step 4: Print the matrix, max-heapify it and then print it again
    printHeapifyPrint(matrix, queueSizes, N, M);

    // Step 5, 6, 7, 8, 9: Emptying and printing the matrix
    emptyMatrix(matrix, queueSizes, N, M);

    return 0;
}


// -------------------  HELPER FUNCTIONS  ------------------- //



/**
 * @brief Reads an integer from the user within a specified range and clamps if out of bounds.
 * 
 * @param msg The message prompt for the user.
 * @param ptr Pointer to store the user input.
 * @param minInc The minimum allowed value (inclusive).
 * @param maxInc The maximum allowed value (inclusive).
 */
void readIntFromUser(const char* msg, int* ptr, int minInc, int maxInc) {
    printf(msg);
    scanf("%d", ptr);
    fflush(stdin);

    // Perform clamping if an invalid number was entered
    if(*ptr > maxInc) *ptr = maxInc;
    if(*ptr < minInc) *ptr = minInc;
}



/**
 * @brief Prints the given matrix.
 * 
 * @param mat The matrix to be printed.
 * @param row The number of rows in the matrix.
 * @param col The number of columns in the matrix.
 */
void printMatrix(int** mat, int row, int col) {
    int i, j;

    for (i = 0; i < row; i++) {
        for(j = 0; j < col; j++) {
            printf("%4d", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}



/**
 * @brief Performs max-heapify operation on an array.
 * 
 * @param arr The array to be heapified.
 * @param n The size of the array.
 * @param si The index of the element to start heapifying from.
 */
void maxHeapifyArray(int* arr, int n, int si) {
    int largest = si;
    int left = 2 * si + 1;
    int right = 2 * si + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != si) {
        int swap = arr[si];
        arr[si] = arr[largest];
        arr[largest] = swap;

        maxHeapifyArray(arr, n, largest);
    }
}



/**
 * @brief Prints the given order array.
 * 
 * @param msg The message to be printed before the array.
 * @param orderArray The array to be printed.
 * @param size The size of the array.
 */
void printOrderArray(const char* msg, int* orderArray, int size) {
    printf(msg);
    if(size == 0) {
        printf("<nothing for now>");
    }
    else {
        int i;
        for (i = 0; i < size; i++) {
            printf("%d", orderArray[i]);
            if(i+1 != size) printf(", "); // print if its not the last iteration 
        }
    }

    printf("\n\n");
}




