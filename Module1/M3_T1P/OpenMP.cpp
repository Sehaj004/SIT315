#include<mpi.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/time.h>
#include<time.h>
#include<omp.h>

#define N 100

void intialiseArray(int array[N][N]); // Function to initialize array with random values
void printArrays(int array[N][N]); // Function to print arrays
void openmpMatrixMultiplication(int np, int rank, int inputArray1[N][N], int inputArray2[N][N], int outputArray[N][N]); // Function to perform matrix multiplication using OpenMP
struct timeval timecheck;

void printOutput(int outputArray[N]){
    for (int i = 0 ; i < N; i++){
        printf(" %d :", outputArray[i]);
    }   
}

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
        intialiseArray(inputArray1); // Fill matrix 1 with random elements
        intialiseArray(inputArray2); // Fill matrix 2 with random elements
    }

    // Timer Start
    if (rank == 0){
        printf("OpenMP MPI Matrix Multiplication.\n");
    }

    gettimeofday(&timecheck, NULL);
    long timeofday_start = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec /1000;

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&inputArray1, N*N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&inputArray2, N*N, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    openmpMatrixMultiplication(np, rank, inputArray1, inputArray2, outputArray);
    MPI_Barrier(MPI_COMM_WORLD);
    
    MPI_Finalize();

    // Timer end
    gettimeofday(&timecheck, NULL);
    long timeofday_end = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec /1000;
    double time_elapsed = timeofday_end - timeofday_start;
    
    if (rank == 0){
        printf("Time taken : %f s\n", time_elapsed/1000);
    }

    return 0;
}

// Function to initialize array with random values
void intialiseArray(int array[N][N]) {
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            array[i][j] = rand() % ((10 - 1) + 1) + 1;
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

// Function to perform matrix multiplication using OpenMP
void openmpMatrixMultiplication(int np, int rank, int inputArray1[N][N], int inputArray2[N][N], int outputArray[N][N]){
    long value;
    int range = N/np;
    int start = rank * range;
    int end = start + range;
    int buffArray[range][N]={0};
  
    #pragma omp parallel
    {  
        #pragma omp for
        for (int i = start ; i < end ; i++)
        {
            for (int j = 0; j < N; j++)
            {
                value = 0;
                for (int k = 0; k < N; k++)
                {
                    value += inputArray1[i][k] * inputArray2[k][j];
                }
                buffArray[i - start][j] = value;
            }
        }
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(buffArray, range * N, MPI_INT, outputArray, range * N, MPI_INT, 0, MPI_COMM_WORLD);
}
