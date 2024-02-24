#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int MAX_SIZE = 100;

void initializeMatrix(int matrix[][MAX_SIZE], int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = rand() % 10; // Initialize with random    values between 0 and 9
        }
    }
}

void multiplyMatrices(int A[][MAX_SIZE], int B[][MAX_SIZE], int C[][MAX_SIZE], int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < size; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void printMatrixToFile(ofstream &file, int matrix[][MAX_SIZE], int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            file << matrix[i][j] << " ";
        }
        file << endl;
    }
}

int main() {
    int N;
    cout << "Enter the size of matrices (N): ";
    cin >> N;

    // Initialize random seed
    srand(time(nullptr));

    int A[MAX_SIZE][MAX_SIZE];
    int B[MAX_SIZE][MAX_SIZE];
    int C[MAX_SIZE][MAX_SIZE];

    // Initialize matrices A and B with random values
    initializeMatrix(A, N);
    initializeMatrix(B, N);

    // Start measuring execution time
    auto start = high_resolution_clock::now();

    // Perform matrix multiplication
    multiplyMatrices(A, B, C, N);

    // Stop measuring execution time
    auto stop = high_resolution_clock::now();

    // Calculate execution time in milliseconds
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Time taken to calculate matrix multiplication: " << duration.count() << " milliseconds" << endl;

    // Write the result matrix C to a file
    ofstream outputFile("output_matrix.txt");
    if (!outputFile) {
        cerr << "Failed to open the output file." << endl;
        return 1;
    }

    outputFile << "Resultant matrix C:" << endl;
    printMatrixToFile(outputFile, C, N);

    cout << "Output written to output_matrix.txt" << endl;

    return 0;
}
