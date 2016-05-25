#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PRIMOS 100000000
#define true 1
#define false 0

char *primos;
long int max;

int crivo(int n)
{
	int i, j, max;
	if (n<=1)
		return -1;
	
	max = (int)sqrt((double)n);
	for (i=2; i < max; i++) {
		if (primos[i] == true) {
			#pragma omp parallel for
			for (j = i * i; j < n; j += i) {
				primos[j] = false;
			}
		}
	}
	
	return 0;
}

int main(int argc, char *argv[])
{
	int i, cycles = 1;
	int nprimes = 0;
	
	switch (argc) {
		case 3:
			cycles = atoi(argv[2]);
			if (cycles < 0 || cycles > 1000)
				exit(1);
		case 2:
			max = atoi(argv[1]);
			if (max < 0 || max > 100000000)
				exit(1);
			
			primos = calloc(max, sizeof(char));
			break;
			
		default:
			exit(1);
	}
	
	
	do{
		#pragma omp parallel for
		for(i=0; i < max; i++){
			primos[i] = true;
		}
		
		crivo(max);
		
		for (i = 2; i < max; i++) {
			if (primos[i] == true)
				nprimes++;
		}
		
		printf("Number of primes: %d\n", nprimes);
		
	} while (cycles-- > 1);
	
	free(primos);
	return 0;
}
	
	