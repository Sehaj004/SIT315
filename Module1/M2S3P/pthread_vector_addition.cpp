#include <chrono>
#include <cstdlib>
#include <iostream>
#include <pthread.h>

using namespace std::chrono;
using namespace std;

#define NUM_THREADS 4

struct ThreadData {
    int* v1;
    int* v2;
    int* v3;
    int size;
    int start;
    int end;
};

void randomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

void* addVectors(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    
    for (int i = data->start; i < data->end; i++) {
        data->v3[i] = data->v1[i] + data->v2[i];
    }
    
    pthread_exit(nullptr);
}

int main() {
    unsigned long size = 100000000;
    srand(time(nullptr));
    int *v1 = new int[size];
    int *v2 = new int[size];
    int *v3 = new int[size];
    randomVector(v1, size);
    randomVector(v2, size);

    pthread_t threads[NUM_THREADS];
    ThreadData td[NUM_THREADS];

    auto start = high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; i++) {
        td[i].v1 = v1;
        td[i].v2 = v2;
        td[i].v3 = v3;
        td[i].size = size;
        td[i].start = i * (size / NUM_THREADS);
        td[i].end = (i + 1) * (size / NUM_THREADS);

        pthread_create(&threads[i], nullptr, addVectors, &td[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], nullptr);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by pthread_vector_addition function: " << duration.count() << " microseconds" << endl;

    delete[] v1;
    delete[] v2;
    delete[] v3;

    return 0;
}
