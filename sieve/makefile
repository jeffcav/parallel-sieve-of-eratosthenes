all:
	gcc sieve.serial.c -o sieve.serial -Wall -lm
	gcc sieve.thread.c -o sieve.thread -Wall -lm -fopenmp
	#gcc four.c -o four -Wall -lm -lpthread
clean:
	rm sieve.serial sieve.thread
