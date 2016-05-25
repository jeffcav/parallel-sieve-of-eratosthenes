#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define true 1
#define false 0
#define MAX_PRIMOS 1000000000

int main(int argc, char *argv[])
{
	int c, m, nprimes;
	int high, low, rem, sqrtn, nprocs, rank, slice;
	char *primes1, *primes2;
	long int N;
	
	switch (argc) {
		case 2:
			N = atoi(argv[1]);
			if (N < 0 || N > MAX_PRIMOS)
				exit(1);
			break;
			
		default:
			N = 500;
	}
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	
	nprimes = 0;
	sqrtn = (int) sqrt(N);
	
	slice = (N - (sqrtn + 1)) / nprocs;
	rem = (N - (sqrtn + 1)) % nprocs;
	low = sqrtn + (rank * slice) + 1;
	high = low + slice - 1;
	
	if (rank == (nprocs - 1) || rank == 0) {
		high += rem;
	}
	
	primes1 = (char*) malloc((sqrtn + 1) * sizeof(char));
	if (primes1 == NULL)
		exit(1);
	
	primes2 = (char*) malloc((high - low + 1) * sizeof(char));
	if (primes2 == NULL)
		exit(1);
	
	/* High is higher than it should be for rank=0, fixing it now.
	 * Reason: making sure we have enough space in *primes2* to receive 
	 * data from any other process.
	 */
	if (rank == 0) {
		high -= rem;
	}
	
	for (c = 0; c <= sqrtn; c++) {
		primes1[c] = true;
	}
	
	for (c = low; c <= high; c++) {
		primes2[c - low] = true;
	}
	
	for (c = 2; c <= sqrtn; c++) {
		if (primes1[c] == false)
			continue;
		
		for (m = c + 1; m <= sqrtn; m++) {
			if (m % c == 0)
				primes1[m] = false;
		}
		
		for (m = low; m <= high; m++){
			if (m % c == 0)
				primes2[m - low] = false;
		}
	}
	
	if (rank == 0) {
		for (c = 2; c <= sqrtn; c++) {
			if (primes1[c] == true) {
				nprimes++;
			}
		}
		
		for (c = low; c <= high; c++) {
			if (primes2[c - low] == true) {
				nprimes++;
			}
		}
		
		for (c = 1; c <= (nprocs - 1); c++) {
			low = sqrtn + (c * slice) + 1;
			high = low + slice - 1;
			if (c == (nprocs - 1))
				high += rem;
			
			MPI_Recv(primes2, high - low + 1, MPI_CHAR, c, 0, MPI_COMM_WORLD,
				 MPI_STATUS_IGNORE);
			
			for (m = low; m <= high; m++) {
				if (primes2[m - low] == true) {
					nprimes++;
				}
			}
		}
		
		printf("\nNumber of primes: %d\n", nprimes);
	} else {
		MPI_Send(primes2, high - low + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}
	
	free(primes1);
	free(primes2);
	
	MPI_Finalize();
	
	return 0;
}
	
	