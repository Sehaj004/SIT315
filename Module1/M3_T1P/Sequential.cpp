#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int N = 600; // Size of matrix to be used

int main()
{
    cout << "Size: " << N << endl; // Print the size of the matrix

    int A[N][N]; // Matrix A
    int B[N][N]; // Matrix B
    int C[N][N]; // Matrix C

    srand(time(0)); // Seed the random number generator

    // Filling all the 2D arrays with random values between 1 to 100
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[i][j] = rand() % 100; // Generate random value for matrix A
            B[i][j] = rand() % 100; // Generate random value for matrix B
        }
    }

    auto start = std::chrono::high_resolution_clock::now(); // Start timing before multiplication

    // Multiplying matrix A and B and storing the result in matrix C
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[k][j]; // Perform matrix multiplication
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now(); // Stop timing after multiplication

    // Calculating duration of multiplication
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "Time taken: " << duration.count() / 1E6 << " seconds" << endl; // Print the time taken in seconds

    return 0;
}
