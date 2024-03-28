#include <iostream>
#include <cstdlib>
#include <chrono>
#include <pthread.h>

using namespace std;
using namespace std::chrono;

const int ARRAY_SIZE = 1000000;
const int NUM_THREADS = 2;

struct ThreadData {
    int* array;
    int start;
    int end;
};

// Function to fill the array with random integers
void fillRandomArray(int array[], int ARRAY_SIZE)
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = rand() % 100; // Generating random numbers between 0 and 99
    }
}

// Function to partition the array into two parts based on a pivot element
int partition(int array[], int start, int end)
{
    int pivot = array[end];
    int partitionIndex = start;
    for (int i = start; i < end; i++)
    {
        if (array[i] < pivot)
        {
            swap(array[i], array[partitionIndex]);
            partitionIndex++;
        }
    }
    swap(array[end], array[partitionIndex]);
    return partitionIndex;
}

// Thread function for quick sort
void* quick_sort_thread(void* arg)
{
    ThreadData* data = (ThreadData*)arg;
    int* array = data->array;
    int start = data->start;
    int end = data->end;
    if (start < end)
    {
        int pIndex = partition(array, start, end);
        quick_sort_thread(new ThreadData{array, start, pIndex - 1});
        quick_sort_thread(new ThreadData{array, pIndex + 1, end});
    }
    return NULL;
}

// Main function to initiate quick sort with multiple threads
void quick_sort(int array[], int start, int end)
{
    pthread_t threads[NUM_THREADS];
    ThreadData* data = new ThreadData[NUM_THREADS];
    int chunkSize = (end - start + 1) / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        data[i] = {array, start + i * chunkSize, (i == NUM_THREADS - 1) ? end : (start + (i + 1) * chunkSize - 1)};
        pthread_create(&threads[i], NULL, quick_sort_thread, &data[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    delete[] data;
}

int main()
{
    int* array = new int[ARRAY_SIZE];

    cout << "Size of the array: " << ARRAY_SIZE << endl;
    cout << "Number of threads used: " << NUM_THREADS << endl;

    srand(time(0)); // Seed for random number generation
    fillRandomArray(array, ARRAY_SIZE); // Filling the array with random integers

    auto start = high_resolution_clock::now(); // Starting the timer before sorting

    quick_sort(array, 0, ARRAY_SIZE - 1); // Performing quick sort with multiple threads

    auto end = high_resolution_clock::now(); // Stopping the timer after sorting

    auto duration = duration_cast<microseconds>(end - start); // Calculating the duration of sorting
    cout << "Time taken for sorting: " << duration.count() << " microseconds" << endl;

    delete[] array; // Freeing the dynamically allocated memory

    return 0;
}
