#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[]) {
    int rank, size;
    char message[] = "Hello World!";
    int message_size = strlen(message) + 1; // Include null terminator

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Master process
    if (rank == 0) {
        // Send message to each worker
        for (int dest_rank = 1; dest_rank < size; dest_rank++) {
            MPI_Send(message, message_size, MPI_CHAR, dest_rank, 0, MPI_COMM_WORLD);
        }
    } else { // Worker processes
        // Receive message from master
        MPI_Recv(message, message_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // Print received message
        printf("Process rank %d received message: %s\n", rank, message);
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}