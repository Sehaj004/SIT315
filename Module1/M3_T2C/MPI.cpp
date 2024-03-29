#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MAX 1000000
int data[MAX];

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Include the MPI library and initialize MPI environment
    srand(time(NULL) + rank);

    // Distribute random seed based on process rank and current time
    int local_size = MAX / size;
    int local_data[local_size];
    for(int i = 0; i < local_size; i++) {
        local_data[i] = rand() % 20;
    }

    // Calculate local sum
    double start_time = MPI_Wtime();
    long local_sum = 0;
    for(int i = 0; i < local_size; i++) {
        local_sum += local_data[i];
    }

    // Reduce local sums to get global sum
    long global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // Stop timer and calculate execution time
    double end_time = MPI_Wtime();

    // Output global sum and execution time
    if(rank == 0) {
        std::cout << "The final sum = " << global_sum << std::endl;
        std::cout << "Execution time = " << end_time - start_time << " seconds" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
