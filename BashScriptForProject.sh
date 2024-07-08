#!/bin/bash

# Set the name of the source code file and the executable
SRC=Project.c
OBJ=mpi_largest_prime_gap

# Number of processes to be spawned
NUM=10

echo "Compiling the source code..."
# Compile the source code
mpicc -o $OBJ $SRC

echo "Compilation complete."
echo "Running the MPI program with $NUM processes..."

# Run the executable file
mpirun -n $NUM ./$OBJ

echo "Execution complete."
echo "Cleaning up..."

# Delete the executable file
rm $OBJ

echo "Cleanup complete."
