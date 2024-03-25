#include <chrono>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <mpi.h>

using namespace std::chrono;
using namespace std;

void randomVector(int vector[], int size) {
  for (int i = 0; i < size; i++) {
    vector[i] = rand() % 100;
  }
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  unsigned long total_size = 100000000;
  unsigned long local_size = total_size / size;

  srand(time(0) + rank); // Add rank to seed for different random numbers

  int *v1 = nullptr, *v2 = nullptr, *v3 = nullptr;
  int *local_v1 = nullptr, *local_v2 = nullptr, *local_v3 = nullptr;

  auto start = high_resolution_clock::now();

  if (rank == 0) {
    v1 = (int *)malloc(total_size * sizeof(int));
    v2 = (int *)malloc(total_size * sizeof(int));
    v3 = (int *)malloc(total_size * sizeof(int));
    randomVector(v1, total_size);
    randomVector(v2, total_size);
  }

  local_v1 = (int *)malloc(local_size * sizeof(int));
  local_v2 = (int *)malloc(local_size * sizeof(int));
  local_v3 = (int *)malloc(local_size * sizeof(int));

  MPI_Scatter(v1, local_size, MPI_INT, local_v1, local_size, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(v2, local_size, MPI_INT, local_v2, local_size, MPI_INT, 0, MPI_COMM_WORLD);

  for (int i = 0; i < local_size; i++) {
    local_v3[i] = local_v1[i] + local_v2[i];
  }

  MPI_Gather(local_v3, local_size, MPI_INT, v3, local_size, MPI_INT, 0, MPI_COMM_WORLD);

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);

  if (rank == 0) {
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
  }

  free(v1);
  free(v2);
  free(v3);
  free(local_v1);
  free(local_v2);
  free(local_v3);

  MPI_Finalize();

  return 0;
}
