#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 1000000

// Function to check if a number is prime
int is_prime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_max_gap = 0, global_max_gap = 0;
    int previous_prime = -1;

    // Each process works on its own segment of numbers
    int segment_size = MAX / size;
    int start = rank * segment_size + 1;
    int end = (rank == size - 1) ? MAX : start + segment_size - 1;

    printf("Process %d working on range %d to %d\n", rank, start, end);

    // Find the largest gap in the assigned segment
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            if (previous_prime != -1) {
                int gap = i - previous_prime;
                if (gap > local_max_gap) {
                    local_max_gap = gap;
                }
            }
            previous_prime = i;
        }
    }

    printf("Process %d found local max gap: %d\n", rank, local_max_gap);

    // Reduce all local max gaps to find the global max gap
    MPI_Reduce(&local_max_gap, &global_max_gap, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("The largest gap between consecutive primes less than %d is %d\n", MAX, global_max_gap);
    }

    MPI_Finalize();
    return 0;
}
