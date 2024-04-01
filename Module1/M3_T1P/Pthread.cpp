#include <iostream>
#include <cstdlib>
#include <chrono>
#include <pthread.h>

using namespace std;
using namespace std::chrono;

#define MAX_THREADS 8

const int N = 500;

int A[N][N]; // Matrix A
int B[N][N]; // Matrix B
int C[N][N]; // Matrix C

int thread_counter = 0;
int chunk_size = N / MAX_THREADS;

// Function to perform matrix multiplication in a specific chunk of rows
void *multiplyMatrix(void *args)
{
    int thread_id = thread_counter++;
    // Compute the chunk of rows for the current thread
    int start_row = thread_id * chunk_size;
    int end_row = (thread_id + 1) * chunk_size;

    for (int i = start_row; i < end_row; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k)
            {
                C[i][j] += A[i][k] * B[k][j]; // Perform matrix multiplication
            }
        }
    }
    return NULL;
}

int main()
{
    cout << "Size of the matrix: " << N << endl; // Print the size of the matrix

    srand(time(0)); // Seed the random number generator

    // Fill all the 2D arrays with random values
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[i][j] = rand() % 100; // Generate random value for matrix A
            B[i][j] = rand() % 100; // Generate random value for matrix B
        }
    }

    auto start = std::chrono::high_resolution_clock::now(); // Start timing before multiplication

    // Multiply matrix A and B and store the result in matrix C using multiple threads
    pthread_t matrix_thread[MAX_THREADS];

    for (int i = 0; i < MAX_THREADS; ++i)
    {
        pthread_create(&matrix_thread[i], NULL, &multiplyMatrix, NULL);
    }

    for (int i = 0; i < MAX_THREADS; ++i)
    {
        pthread_join(matrix_thread[i], NULL);
    }

    auto end = std::chrono::high_resolution_clock::now(); // Stop timing after multiplication

    // Calculate duration of multiplication
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "Time taken: " << duration.count() / 1e6 << " seconds" << endl; // Print the time taken in seconds

    return 0;
}
