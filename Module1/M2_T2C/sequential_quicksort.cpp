#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int ARRAY_SIZE = 1000000;

// Function to fill the array with random integers
void fillRandomArray(int array[], int ARRAY_SIZE)
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = rand() % 100; // Generating random numbers between 0 and 99
    }
}

// Function to partition the array into two parts based on a pivot element
int partition(int array[], int start, int end, int ARRAY_SIZE)
{
    int pivot = array[end]; // Selecting the pivot element as the last element
    int partitionIndex = start;

    for (int i = start; i < end; i++)
    {
        if (array[i] < pivot) // Swapping if the current element is smaller than the pivot
        {
            swap(array[i], array[partitionIndex]);
            partitionIndex++; // Incrementing the partition index
        }
    }
    swap(array[end], array[partitionIndex]); // Moving the pivot to its correct position
    return partitionIndex;
}

// Recursive function to perform quick sort
void quick_sort(int array[], int start, int end, int ARRAY_SIZE)
{
    if (start < end)
    {
        int pIndex = partition(array, start, end, ARRAY_SIZE); // Index of the pivot element after partitioning
        quick_sort(array, start, pIndex - 1, ARRAY_SIZE);      // Sorting elements smaller than the pivot
        quick_sort(array, pIndex + 1, end, ARRAY_SIZE);        // Sorting elements greater than the pivot
    }
}

int main()
{
    int *array = new int[ARRAY_SIZE];

    cout << "Size of the array: " << ARRAY_SIZE << endl;

    srand(time(0)); // Seed for random number generation
    fillRandomArray(array, ARRAY_SIZE); // Filling the array with random integers

    auto start = std::chrono::high_resolution_clock::now(); // Starting the timer before sorting

    quick_sort(array, 0, ARRAY_SIZE - 1, ARRAY_SIZE); // Performing quick sort

    auto end = std::chrono::high_resolution_clock::now(); // Stopping the timer after sorting

    // Calculating the duration of sorting
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "Time taken for sorting: " << duration.count() << " microseconds" << endl;

    delete[] array; // Freeing the dynamically allocated memory
    return 0;
}
