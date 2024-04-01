#include<mpi.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include <sys/time.h>
#include <time.h>

#define N 100

// Function prototypes
void intialiseArray(int array[N][N]); // Initialize an array with random values
void printArrays(int array[N][N]); // Print array elements
void MatrixMultiplication(int np, int rank, int inputArray1[N][N], int inputArray2[N][N], int outputArray[N][N]); // Perform matrix multiplication
void printOutput(int outputArray[N]); // Print the output array

// Global variable to measure time
struct timeval timecheck;

int main(){
    MPI_Init(NULL, NULL);

    int np = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &np);     // Get the number of nodes
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);    // Get the rank of the process

    int inputArray1[N][N], inputArray2[N][N];
    int outputArray[N][N]={{0}};

    printf("Size of Matrix: %d\n", N); // Print the size of the matrix

    if (rank == 0) {
        printf("Filling matrices with random elements...\n");
        intialiseArray(inputArray1); // Fill matrix 1 with random elements
        intialiseArray(inputArray2); // Fill matrix 2 with random elements
    }

    // Timer start
    if (rank == 0){
        printf("MPI Matrix Multiplication...\n");
    }

    // Start timing
    gettimeofday(&timecheck, NULL);
    long timeofday_start = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec /1000;

    // Broadcast input matrices to all processes
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&inputArray1, N*N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&inputArray2, N*N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    // Perform matrix multiplication
    MatrixMultiplication(np, rank, inputArray1, inputArray2, outputArray);
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize(); // Finalize MPI

    // End timing
    gettimeofday(&timecheck, NULL);
    long timeofday_end = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec /1000;
    double time_elapsed = timeofday_end - timeofday_start;

    if (rank == 0){
        printf("Time taken: %f s\n", time_elapsed/1000); // Print the time taken for computation
    }

    return 0;
}

// Function to initialize array with random values
void intialiseArray(int array[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            array[i][j] = rand() % ((10 - 1) + 1) + 1; // Generate random numbers between 1 and 10
        }
    }
}

// Function to print arrays
void printArrays(int array[N][N]){
    printf("[");
    for (int i = 0; i < N; i++) {
        printf("[");
        for (int j = 0; j < N; j++) {
            printf("%i", array[i][j]);
            printf(" ");
        }
        printf("]\n");
    }
    printf("]\n\n");
}

// Function to perform matrix multiplication
void MatrixMultiplication(int np, int rank, int inputArray1[N][N], int inputArray2[N][N], int outputArray[N][N]){
    long value;
    int range = N/np;
    int start = rank * range;
    int end = start + range;
    int buffArray[range][N]={0};

    // Perform matrix multiplication locally
    for (int i = start ; i < end ; i++) {
        for (int j = 0; j < N; j++) {
            value = 0;
            for (int k = 0; k < N; k++) {
                value += inputArray1[i][k] * inputArray2[k][j];
            }
            buffArray[i - start][j] = value;
        }
    }

    // Gather results from all processes
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(buffArray, range * N, MPI_INT, outputArray, range * N, MPI_INT, 0, MPI_COMM_WORLD);
}
