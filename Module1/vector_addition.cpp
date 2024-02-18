#include <chrono> //library for time measurements
#include <cstdlib> //library for memory allocation
#include <iostream> //library for input and output
#include <time.h> //library for time-related functions

using namespace std::chrono; //namespace for time measurements
using namespace std; //standard namespace

// Function to generate random values for the vectors
void randomVector(int vector[], int size) {
  for (int i = 0; i < size; i++) {
    vector[i] = rand() % 100; //Generate a random value between 0 and 99
  }
}

int main() {
  unsigned long size = 100000000; // Size of the vectors
  srand(time(0)); // Seed for random number generation
  int *v1, *v2, *v3;
  auto start = high_resolution_clock::now(); // Start time measurement
  v1 = (int *)malloc(size * sizeof(int)); // Allocate memory for vector 1
  v2 = (int *)malloc(size * sizeof(int)); // Allocate memory for vector 2
  v3 = (int *)malloc(size * sizeof(int)); // Allocate memory for vector 3
  randomVector(v1, size); // Fill vector 1 with random values
  randomVector(v2, size); // Fill vector 2 with random values

  // Perform vector addition
  for (int i = 0; i < size; i++) {
    v3[i] = v1[i] + v2[i];
  }
  auto stop = high_resolution_clock::now(); // Stop time measurement
  auto duration = duration_cast<microseconds>(stop - start); // Calculate duration
  cout << "Time taken by function :" << duration.count() << " microseconds"
       << endl; // Output duration
  return 0;
}
