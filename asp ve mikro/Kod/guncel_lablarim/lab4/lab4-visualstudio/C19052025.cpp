#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>


// Function to print a matrix stored in a 1D array
void print_matrix(unsigned* matrix, unsigned rows, unsigned cols, FILE* file);
// Function to read matrix from a file
void read_matrix(const char* filename, unsigned** matrix, unsigned* rows, unsigned* cols);
// Function to read kernel from a file
void read_kernel(const char* filename, unsigned** kernel, unsigned* k);
// Function to write output matrix to a file
void write_output(const char* filename, unsigned* output, unsigned rows, unsigned cols);
// Initialize output as zeros.
void initialize_output(unsigned*, unsigned, unsigned);

int main() {

    unsigned n, m, k;  // n = rows of matrix, m = cols of matrix, k = kernel size
    // Dynamically allocate memory for matrix, kernel, and output
    unsigned* matrix = NULL;  // Input matrix
    unsigned* kernel = NULL;  // Kernel size 3x3
    unsigned* output = NULL;  // Max size of output matrix

    char matrix_filename[30];
    char kernel_filename[30];

    // Read the file names
    printf("Enter matrix filename: ");
    scanf("%s", matrix_filename);
    printf("Enter kernel filename: ");
    scanf("%s", kernel_filename);

    // Read matrix and kernel from files
    read_matrix(matrix_filename, &matrix, &n, &m);  // Read matrix from file
    read_kernel(kernel_filename, &kernel, &k);      // Read kernel from file

    // For simplicity we say: padding = 0, stride = 1
    // With this setting we can calculate the output size
    unsigned output_rows = n - k + 1;
    unsigned output_cols = m - k + 1;
    output = (unsigned*)malloc(output_rows * output_cols * sizeof(unsigned));
    initialize_output(output, output_rows, output_cols);

    // Print the input matrix and kernel
    printf("Input Matrix: ");
    print_matrix(matrix, n, m, stdout);

    printf("\nKernel: ");
    print_matrix(kernel, k, k, stdout);
    
    // --------------- KOD BAŞLANGICI --------------- //

    unsigned i, j, ki, kj; // kullandığım 4 for loop için indeksler
    unsigned mval, kval, sum; // konvolüsyon hesabında kullanılan değişkenler

    // Referans için yazdığım C kodu
    /*
    for (int i = 0; i < output_rows; i++) {
        for (int j = 0; j < output_cols; j++) {
            unsigned sum = 0;
            for (int ki = 0; ki < k; ki++) {
                for (int kj = 0; kj < k; kj++) {
                    sum += matrix[(i + ki) * m + (j + kj)] * kernel[ki * k + kj];
                }
            }
            output[i * output_cols + j] = sum;
        }
    }
    */

    __asm {
        MOV i, 0
FOR1_START:
        MOV EAX, output_rows
        CMP i, EAX                   // i < output_rows
        JAE FOR1_END
        // i döngüsü kodu başla
        MOV j, 0
FOR2_START:
        MOV EAX, output_cols
        CMP j, EAX                   // j < output_cols
        JAE FOR2_END
        // j döngüsü kodu başla
        MOV ki, 0
        MOV sum, 0
FOR3_START:
        MOV EAX, k
        CMP ki, EAX                  // ki < k
        JAE FOR3_END
        // ki döngüsü kodu başla
        MOV kj, 0
FOR4_START:
        MOV EAX, k
        CMP kj, EAX                  // kj < k
        JAE FOR4_END
        // kj döngüsü kodu başla

        // unsigned mval = matrix[(i + ki) * m + (j + kj)];
        MOV EAX, i            // EAX = i
        ADD EAX, ki           // EAX = i+ki
        MOV EBX, m
        MUL EBX               // EAX = (i+ki)*m
        ADD EAX, j            // EAX = (i+ki)*m + j
        ADD EAX, kj           // EAX = (i+ki)*m + j + kj
        SHL EAX, 2            // EAX = o indeksin byte karşılığı
        MOV EBX, matrix
        MOV EBX, [EBX + EAX]  // EBX = matrix[(i + ki) * m + (j + kj)]
        MOV mval, EBX

        // unsigned kval = kernel[ki * k + kj];
        MOV EAX, ki           // EAX = ki
        MOV EBX, k
        MUL EBX               // EAX = ki*k
        ADD EAX, kj           // EAX = ki*k + kj
        SHL EAX, 2            // EAX = o indeksin byte karşılığı
        MOV EBX, kernel
        MOV EBX, [EBX + EAX]  // EBX = kernel[ki * k + kj]
        MOV kval, EBX

        // sum += mval * kval;
        MOV EAX, mval   // EAX = mval
        MOV EBX, kval
        MUL EBX         // EAX = mval * kval
        ADD sum, EAX
        
        // kj döngüsü kodu bitiş
        INC kj
        JMP FOR4_START
FOR4_END:
        // ki döngüsü kodu bitiş
        INC ki
        JMP FOR3_START
FOR3_END:
        // output[i * output_cols + j] = sum;
        MOV EAX, i             // EAX = i
        MOV EBX, output_cols
        MUL EBX                // EAX = i*output_cols
        ADD EAX, j             // EAX = i*output_cols+j
        SHL EAX, 2             // EAX = o indeksin byte karşılığı
        MOV EBX, output
        MOV ECX, sum
        MOV [EBX + EAX], ECX
        
        // j döngüsü kodu bitiş
        INC j
        JMP FOR2_START
FOR2_END:
        // i döngüsü kodu bitiş
        INC i
        JMP FOR1_START
FOR1_END:
    }

    // --------------- KOD BİTİŞİ --------------- //

    // Write result to output file
    write_output("./output.txt", output, output_rows, output_cols);

    // Print result
    printf("\nOutput matrix after convolution: ");
    print_matrix(output, output_rows, output_cols, stdout);

    // Free allocated memory
    free(matrix);
    free(kernel);
    free(output);

    return 0;
}

void print_matrix(unsigned* matrix, unsigned rows, unsigned cols, FILE* file) {
    if (file == stdout) {
        printf("(%ux%u)\n", rows, cols);
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%u ", matrix[i * cols + j]);
        }
        fprintf(file, "\n");
    }
}

void read_matrix(const char* filename, unsigned** matrix, unsigned* rows, unsigned* cols) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    // Read dimensions
    fscanf(file, "%u %u", rows, cols);
    *matrix = (unsigned*)malloc(((*rows) * (*cols)) * sizeof(unsigned));

    // Read matrix elements
    for (int i = 0; i < (*rows); i++) {
        for (int j = 0; j < (*cols); j++) {
            fscanf(file, "%u", &(*matrix)[i * (*cols) + j]);
        }
    }

    fclose(file);
}

void read_kernel(const char* filename, unsigned** kernel, unsigned* k) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    // Read kernel size
    fscanf(file, "%u", k);
    *kernel = (unsigned*)malloc((*k) * (*k) * sizeof(unsigned));

    // Read kernel elements
    for (int i = 0; i < (*k); i++) {
        for (int j = 0; j < (*k); j++) {
            fscanf(file, "%u", &(*kernel)[i * (*k) + j]);
        }
    }

    fclose(file);
}

void write_output(const char* filename, unsigned* output, unsigned rows, unsigned cols) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    // Write dimensions of the output matrix
    fprintf(file, "%u %u\n", rows, cols);

    // Write output matrix elements
    print_matrix(output, rows, cols, file);

    fclose(file);
}

void initialize_output(unsigned* output, unsigned output_rows, unsigned output_cols) {
    int i;
    for (i = 0; i < output_cols * output_rows; i++)
        output[i] = 0;
    
}

