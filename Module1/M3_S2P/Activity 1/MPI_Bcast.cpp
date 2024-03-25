#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[]) {
    int rank, size;
    char message[100]; // Increase buffer size to accommodate received message
    int message_size = 100; // Set the maximum size for the message buffer

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Master process
    if (rank == 0) {
        strcpy(message, "Hello World!"); // Copy message to be broadcasted
    }

    // Broadcast message from master to all other processes
    MPI_Bcast(message, message_size, MPI_CHAR, 0, MPI_COMM_WORLD);

    // Print received message for all processes
    printf("Process rank %d received message: %s\n", rank, message);

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}