#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <ctime>

using namespace std;
using namespace std::chrono;

// Function to generate random values for the vectors
void randomVector(int vector[], int size) {
    for (int i = 0; i < size; ++i) {
        vector[i] = rand() % 100; // Generate a random value between 0 and 99
    }
}

// Function for parallel vector addition
void parallelVectorAddition(int *v1, int *v2, int *v3, int size, int start, int end) {
    for (int i = start; i < end; ++i) {
        v3[i] = v1[i] + v2[i];
    }
}

int main() {
    unsigned long size = 100000000; // Size of the vectors
    srand(time(0)); // Seed for random number generation

    int *v1 = new int[size]; // Vector 1
    int *v2 = new int[size]; // Vector 2
    int *v3 = new int[size]; // Result Vector

    randomVector(v1, size); // Fill vector 1 with random values
    randomVector(v2, size); // Fill vector 2 with random values

    auto start = high_resolution_clock::now(); // Start time measurement

    // Number of threads
    int num_threads = thread::hardware_concurrency();
    vector<thread> threads;

    // Divide the work among threads
    int chunk_size = size / num_threads;
    for (int i = 0; i < num_threads; ++i) {
        int start_index = i * chunk_size;
        int end_index = (i == num_threads - 1) ? size : (i + 1) * chunk_size;
        threads.emplace_back(parallelVectorAddition, v1, v2, v3, size, start_index, end_index);
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    auto stop = high_resolution_clock::now(); // Stop time measurement
    auto duration = duration_cast<microseconds>(stop - start); // Calculate duration

    cout << "Time taken by function: " << duration.count() << " microseconds" << endl; // Output duration

    delete[] v1; // Free allocated memory for vector 1
    delete[] v2; // Free allocated memory for vector 2
    delete[] v3; // Free allocated memory for vector 3

    return 0;
}
