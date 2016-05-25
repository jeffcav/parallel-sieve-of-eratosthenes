all:
	gcc sieve.serial.c -o sieve.serial -Wall -lm
	gcc sieve.thread.c -o sieve.thread -Wall -lm -fopenmp
	mpicc -o sieve.process sieve.process.c -lm -Wall
	#gcc four.c -o four -Wall -lm -lpthread
clean:
	rm sieve.serial sieve.thread sieve.process
