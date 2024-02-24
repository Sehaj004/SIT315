#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <pthread.h>

using namespace std;
using namespace std::chrono;

const int MAX_SIZE = 100;
const int NUM_THREADS = 10; // Number of threads to use for parallelization

// Structure to pass arguments to the thread function
struct ThreadData {
    int start_row;
    int end_row;
    int size;
    int (*A)[MAX_SIZE];
    int (*B)[MAX_SIZE];
    int (*C)[MAX_SIZE];
};

// Matrix initialization function
void initializeMatrix(int matrix[][MAX_SIZE], int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = rand() % 10; // Initialize with random values between 0 and 9
        }
    }
}

// Matrix multiplication function for a specific portion of the matrix
void* multiplyMatrix(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    for (int i = data->start_row; i < data->end_row; ++i) {
        for (int j = 0; j < data->size; ++j) {
            data->C[i][j] = 0;
            for (int k = 0; k < data->size; ++k) {
                data->C[i][j] += data->A[i][k] * data->B[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

// Function to print the matrix to a file
void printMatrixToFile(ofstream& file, int matrix[][MAX_SIZE], int size) {
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

    // Create threads for matrix multiplication
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) {
        int start_row = i * (N / NUM_THREADS);
        int end_row = (i + 1) * (N / NUM_THREADS);
        thread_data[i] = {start_row, end_row, N, A, B, C};
        pthread_create(&threads[i], NULL, multiplyMatrix, (void*)&thread_data[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

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
