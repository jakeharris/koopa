# Compiler
CC = g++

# Compiler flags
CFLAGS = -c

koopa: job.h process.h koopa.h koopa.cpp
	$(CC) job.h process.h koopa.h koopa.cpp -o koopa

clean:
	rm -Rf *.o koopa
