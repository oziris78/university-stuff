

/*
 * Algoritma Analizi - Ödev #4
 * Oğuzhan Topaloğlu, Ç19052025
*/

// Brute force için kullanılan kaynak:
//   https://jsedano.dev/algorithms/2020/12/30/n-queens-brute-force.html
// permute(...) fonksiyonu için kullanılan kaynak:
//   https://stackoverflow.com/questions/16989689/print-all-the-permutations-of-a-string-in-c


// Aşağıdaki makro ile sonuçların yazılıp yazılmamasını belirtebilirsiniz
//   hiçbir sonuç yazılmasın istiyorsanız makroyu yoruma alın
// #define PRINT_BOARDS


#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>


// Brute Force fonksiyonları
void permute(int, int*, int, int*);
double solveBruteForce(int, int*);

// Optimized_1 fonksiyonları
void optimized1Internal(int*, int, int, int*);
double solveOptimized1(int, int*);

// Optimized_2 fonksiyonları
void optimized2Internal(int*, int, int*, int, int*);
double solveOptimized2(int, int*);

// Backtracking fonksiyonları
void backtrackingInternal(int*, int, int*, int*, int*, int, int*);
double solveBacktracking(int, int*);

// Helper fonksiyonlar
void printSolution(const char*, int*, int, int);
void checkSolution(const char*, int*, int, int*);


///////////////////////// BRUTE FORCE FONKSİYONLARI /////////////////////////


/**
 * @brief N-Queens problemini brute force yöntemi ile çözmek için tüm permütasyonları hesaplar
 * 
 * @param index Geçerli permütasyonun başlangıç noktası
 * @param arr Üzerinde permütasyon yapılan dizi
 * @param n Tahtadaki toplam hücre sayısı
 * @param solutionCount Bulunan çözüm sayısı
 */
void permute(int index, int* arr, int n, int* solutionCount) {
    int i, temp;
    if (index == n-1) { // Permütasyon hesaplandı
        checkSolution("Brute Force", arr, n, solutionCount);
        return;
    }
    for (i = index; i < n; i++) {
        temp = arr[index];
        arr[index] = arr[i];
        arr[i] = temp;

        permute(index+1, arr, n, solutionCount);
        
        temp = arr[i];
        arr[i] = arr[index];
        arr[index] = temp;
    }
   return;
}


/**
 * @brief N-Queens problemini brute force yöntemi ile çözer
 * 
 * @param N Tahtanın boyutu (NxN)
 * @param solutionCount Bulunan çözüm sayısını döndürmek için kullanılan değişken
 * @return double Çözüm için harcanan süre
 */
double solveBruteForce(int N, int* solutionCount) {
    // 0,1,2,3,...,N^2-1 permütasyon
    int* arr = (int*) malloc((N*N-1) *  sizeof(int));
    int i;
    for(i = 0; i < N*N-1; i++) {
        arr[i] = i;
    }

    clock_t start = clock();
    permute(0, arr, N, solutionCount); // çözüm bulunca solutionCount++ yapacak
    clock_t end = clock();

    double dt = ((double)(end - start)) / CLOCKS_PER_SEC;
    free(arr);
    return dt;
}


///////////////////////// OPTIMIZED_1 FONKSİYONLARI /////////////////////////


/**
 * @brief N-Queens problemini optimized_1 ile çözmek için recursive bir fonksiyon
 * 
 * @param board Geçerli tahta durumunu temsil eden dizi
 * @param row Kontrol edilen geçerli satır
 * @param N Tahtanın boyutu (NxN)
 * @param solutionCount Bulunan çözüm sayısı
 */
void optimized1Internal(int* board, int row, int N, int* solutionCount) {
    if (row == N) { // satırlar bitti
        checkSolution("Optimized_1", board, N, solutionCount);
        return;
    }

    int col;
    for (col = 0; col < N; col++) {
        int valid = 1;

        // Daha önceki satırlardaki sütunları kontrol et
        int prevRow = 0;
        while(prevRow < row && valid == 1) {
            if (board[prevRow] == col || abs(prevRow - row) == abs(board[prevRow] - col)) {
                valid = 0;
            }
            
            prevRow++;
        }

        if (valid) {
            board[row] = col; // Satıra veziri yerleştir
            optimized1Internal(board, row + 1, N, solutionCount);
        }
    }
}


/**
 * @brief N-Queens problemini optimized_1 ile çözer
 * 
 * @param N Tahtanın boyutu (NxN)
 * @param solutionCount Bulunan çözüm sayısını döndürmek için kullanılan değişken
 * @return double Çözüm için harcanan süre
 */
double solveOptimized1(int N, int* solutionCount) {
    int* board = (int*) malloc(N * sizeof(int)); // Sadece satır başına bir vezir tutuyoruz
    
    clock_t start = clock();
    optimized1Internal(board, 0, N, solutionCount);
    clock_t end = clock();

    double dt = ((double)(end - start)) / CLOCKS_PER_SEC;
    free(board);
    return dt;
}


///////////////////////// OPTIMIZED_2 FONKSİYONLARI /////////////////////////


/**
 * @brief N-Queens problemini optimized_2 ile çözmek için recursive bir fonksiyon
 * 
 * @param board Geçerli tahta durumunu temsil eden dizi
 * @param row Kontrol edilen geçerli satır
 * @param columns Kullanılan sütunları takip eden dizi
 * @param N Tahtanın boyutu (NxN)
 * @param solutionCount Bulunan çözüm sayısı
 */
void optimized2Internal(int* board, int row, int* columns, int N, int* solutionCount) {
    if (row == N) { // satırlar bitti
        checkSolution("Optimized_2", board, N, solutionCount);
        return;
    }

    int col;
    for (col = 0; col < N; col++) {
        int valid = 1;

        // Bu sütun zaten dolu
        if (columns[col]) {
            valid = 0;  
        }

        // Daha önceki satırlardaki sütunları kontrol et
        int prevRow = 0;
        while (prevRow < row && valid == 1) {
            if (board[prevRow] == col || abs(prevRow - row) == abs(board[prevRow] - col)) {
                valid = 0;
            }
            
            prevRow++;
        }

        if (valid) {
            // Veziri (row, col) konumuna yerleştir
            board[row] = col;
            columns[col] = 1;  // Sütunu işaretle

            // Bir sonraki satır için recursive çağrı
            optimized2Internal(board, row + 1, columns, N, solutionCount);

            // Backtrack: Sütunun işaretini kaldır
            columns[col] = 0;
        }
    }
}


/**
 * @brief N-Queens problemini optimized_2 ile çözer
 * 
 * @param N Tahtanın boyutu (NxN)
 * @param solutionCount Bulunan çözüm sayısını döndürmek için kullanılan değişken
 * @return double Çözüm için harcanan süre
 */
double solveOptimized2(int N, int* solutionCount) {
    int* board = (int*) malloc(N * sizeof(int));
    int* columns = (int*) calloc(N, sizeof(int));

    clock_t start = clock();
    optimized2Internal(board, 0, columns, N, solutionCount);
    clock_t end = clock();

    double dt = ((double)(end - start)) / CLOCKS_PER_SEC;
    free(board);
    free(columns);
    return dt;
}


///////////////////////// BACKTRACKING FONKSİYONLARI /////////////////////////


/**
 * @brief N-Queens problemini backtracking yöntemi ile çözmek için recursive bir fonksiyon
 * 
 * @param board Geçerli tahta durumunu temsil eden dizi
 * @param row Kontrol edilen geçerli satır
 * @param columns Kullanılan sütunları saklayan dizi
 * @param diagonal1 Kullanılan sol çaprazları saklayan dizi
 * @param diagonal2 Kullanılan sağ çaprazları saklayan dizi
 * @param N Tahtanın boyutu (NxN)
 * @param solutionCount Bulunan çözüm sayısı
 */
void backtrackingInternal(int* board, int row, int* columns, int* diagonal1, int* diagonal2, int N, int* solutionCount) 
{
    // satırlar bitti, KESİN çözüm bulundu doğrudan print et
    if (row == N) {
        printSolution("Backtracking", board, N, *solutionCount);
        (*solutionCount)++;  
        return;
    }

    int col;
    for (col = 0; col < N; col++) {
        // Sütunları ve çaprazları kontrol et
        int invalid = columns[col] || diagonal1[row - col + N - 1] || diagonal2[row + col];
        if (!invalid) {
            board[row] = col;  // Veziri yerleştir
            columns[col] = 1;  // Sütunu işaretle
            diagonal1[row - col + N - 1] = 1;  // Sol çaprazı işaretle
            diagonal2[row + col] = 1;  // Sağ çaprazı işaretle

            backtrackingInternal(board, row + 1, columns, diagonal1, diagonal2, N, solutionCount);

            // Backtrack: sütun ve çaprazların işaretlerini kaldır
            columns[col] = 0;
            diagonal1[row - col + N - 1] = 0;
            diagonal2[row + col] = 0;
        }
    }
}


/**
 * @brief N-Queens problemini backtracking yöntemi ile çözer
 * 
 * @param N Tahtanın boyutu (NxN)
 * @param solutionCount Bulunan çözüm sayısını döndürmek için kullanılan değişken
 * @return double Çözüm için harcanan süre
 */
double solveBacktracking(int N, int* solutionCount) {
    int* board = (int*) calloc(N, sizeof(int));
    int* columns = (int*) calloc(N, sizeof(int));
    int* diagonal1 = (int*) calloc(2 * N - 1, sizeof(int));
    int* diagonal2 = (int*) calloc(2 * N - 1, sizeof(int));

    clock_t start = clock();
    backtrackingInternal(board, 0, columns, diagonal1, diagonal2, N, solutionCount);
    clock_t end = clock();

    double dt = ((double)(end - start)) / CLOCKS_PER_SEC;
    free(board);
    free(columns);
    free(diagonal1);
    free(diagonal2);
    return dt;
}


////////////////////////////////// MAIN //////////////////////////////////


int main() {
    int option, N, stop = 0;

    while (!stop) {
        // option'ı kullanıcıdan oku
        const int OPTION_COUNT = 6;
        int validOption = 0;
        while (!validOption) {
            printf("\n--------- Solving options ---------\n");
            printf("0- Solve using BRUTE FORCE\n");
            printf("1- Solve using OPTIMIZED_1\n");
            printf("2- Solve using OPTIMIZED_2\n");
            printf("3- Solve using BACKTRACKING\n");
            printf("4- Solve using ALL ALGORITHMS\n");
            printf("5- Solve using ALL ALGORITHMS excluding Brute Force\n");
            printf("6- Terminate the program\n");
            printf("Choose an option [0, %d]: ", OPTION_COUNT);
            scanf("%d", &option);

            if (option >= 0 && option <= OPTION_COUNT) {
                validOption = 1;
            } 
            else {
                printf("Invalid option, try again.\n");
            }
        }

        if (option != OPTION_COUNT) {
            int validInput = 0;
            while (!validInput) {
                printf("Enter N value: ");
                scanf("%d", &N);

                if (N >= 4) {
                    validInput = 1;
                } else {
                    printf("Invalid N value, enter 4 or above.\n");
                }
            }

            int sc = 0;
            if (option == 0) {
                double dt = solveBruteForce(N, &sc);
                printf("BRUTE FORCE   => N: %d, solCount: %d, timeInSecs: %.3f\n", N, sc, dt);
            } 
            else if (option == 1) {
                double dt = solveOptimized1(N, &sc);
                printf("OPTIMIZED_1   => N: %d, solCount: %d, timeInSecs: %.3f\n", N, sc, dt);
            } 
            else if (option == 2) {
                double dt = solveOptimized2(N, &sc);
                printf("OPTIMIZED_2   => N: %d, solCount: %d, timeInSecs: %.3f\n", N, sc, dt);
            } 
            else if (option == 3) {
                double dt = solveBacktracking(N, &sc);
                printf("BACKTRACKING  => N: %d, solCount: %d, timeInSecs: %.3f\n", N, sc, dt);
            } 
            else if (option == 4) {
                int sc1 = 0, sc2 = 0, sc3 = 0, sc4 = 0;
                double dt1 = solveBruteForce(N, &sc1);
                double dt2 = solveOptimized1(N, &sc2);
                double dt3 = solveOptimized2(N, &sc3);
                double dt4 = solveBacktracking(N, &sc4);
                
                printf("BRUTE FORCE   => N: %d, solCount: %d, timeInSecs: %.3f\n", N, sc1, dt1);
                printf("OPTIMIZED_1   => N: %d, solCount: %d, timeInSecs: %.3f\n", N, sc2, dt2);
                printf("OPTIMIZED_2   => N: %d, solCount: %d, timeInSecs: %.3f\n", N, sc3, dt3);
                printf("BACKTRACKING  => N: %d, solCount: %d, timeInSecs: %.3f\n", N, sc4, dt4);
            }
            else if (option == 5) {
                int sc2 = 0, sc3 = 0, sc4 = 0;
                double dt2 = solveOptimized1(N, &sc2);
                double dt3 = solveOptimized2(N, &sc3);
                double dt4 = solveBacktracking(N, &sc4);
                
                printf("OPTIMIZED_1   => N: %d, solCount: %d, timeInSecs: %.3f\n", N, sc2, dt2);
                printf("OPTIMIZED_2   => N: %d, solCount: %d, timeInSecs: %.3f\n", N, sc3, dt3);
                printf("BACKTRACKING  => N: %d, solCount: %d, timeInSecs: %.3f\n", N, sc4, dt4);
            }
        }
        else {
            stop = 1;
        }
    }

    printf("Terminating the program...\n");
    return 0;
}


///////////////////////////// HELPER FONKSİYONLAR /////////////////////////////


/**
 * @brief Belirtilen algoritmanın bulduğu çözümleri yazdırır
 * 
 * @param typeStr Algoritmanın adı
 * @param board Çözüm tahtasını temsil eden dizi
 * @param N Tahtanın boyutu (NxN)
 * @param solutionCount Bulunan çözüm sayısı
 */
void printSolution(const char* typeStr, int* board, int N, int solutionCount) {
#ifdef PRINT_BOARDS
    int i, j;
    if(solutionCount == 0) // çıktı güzel gözüksün diye
        printf("\n");

    printf("%s Solution #%d:\n", typeStr, solutionCount+1);
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++) {
            printf(j != board[i] ? ". " : "Q ");
        }
        printf("\n");
    }
    printf("\n");
#endif
}


/**
 * @brief Bir çözümün geçerliliğini kontrol eder ve geçerli ise yazdırır
 * 
 * @param typeStr Algoritmanın adı
 * @param board Çözüm tahtasını temsil eden dizi
 * @param N Tahtanın boyutu (NxN)
 * @param solutionCount Bulunan çözüm sayısını tutan değişken
 */
void checkSolution(const char* typeStr, int* board, int N, int* solutionCount) {
    int i, j;
    for (i = 0; i < N - 1; i++) {
        for (j = i + 1; j < N; j++) {
            // Çapraz saldırıları kontrol et
            if (abs(i - j) == abs(board[i] - board[j])) {
                return; // Çözüm değil
            }
        }
    }

    printSolution(typeStr, board, N, *solutionCount);
    (*solutionCount)++;
}

