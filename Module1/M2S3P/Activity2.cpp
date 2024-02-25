#include <chrono>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <omp.h>

using namespace std::chrono;
using namespace std;

void randomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

int main() {
    unsigned long size = 100000000;
    srand(time(0));
    int *v1, *v2, *v3;
    auto start = high_resolution_clock::now();
    v1 = (int *)malloc(size * sizeof(int));
    v2 = (int *)malloc(size * sizeof(int));
    v3 = (int *)malloc(size * sizeof(int));
    randomVector(v1, size);
    randomVector(v2, size);

    // Step 1: Adding default(none) and adjusting data sharing
    #pragma omp parallel default(none) shared(v1, v2, v3, size)
    {
        #pragma omp for
        for (int i = 0; i < size; i++) {
            v3[i] = v1[i] + v2[i];
        }
    }

    // Step 2: Computing total sum using atomic update
    int total = 0;
    #pragma omp parallel for default(none) shared(v3, size) reduction(+:total)
    for (int i = 0; i < size; i++) {
        total += v3[i];
    }
    cout << "Total sum using atomic update: " << total << endl;

    // Step 3: Using the reduction clause to compute the total sum
    int total_reduction = 0;
    #pragma omp parallel for default(none) shared(v3, size) reduction(+:total_reduction)
    for (int i = 0; i < size; i++) {
        total_reduction += v3[i];
    }
    cout << "Total sum using reduction clause: " << total_reduction << endl;

    // Step 4: Implementing an alternative version using a critical section
    int total_critical = 0;
    #pragma omp parallel default(none) shared(v3, size) private(total_critical)
    {
        int local_total = 0;
        #pragma omp for
        for (int i = 0; i < size; i++) {
            local_total += v3[i];
        }
        #pragma omp critical
        total_critical += local_total;
    }
    cout << "Total sum using critical section: " << total_critical << endl;

    // Step 5: Trying different OpenMP scheduling techniques
    #pragma omp parallel for default(none) shared(v1, v2, v3, size) private(i) schedule(static, 1000)
    for (int i = 0; i < size; i++) {
        v3[i] = v1[i] + v2[i];
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by parallel_vector_addition function: " << duration.count() << " microseconds" << endl;

    free(v1);
    free(v2);
    free(v3);

    return 0;
}
