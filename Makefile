# Compiler
CC = g++

# Compiler flags
CFLAGS = -c

koopa: job.h process.h koopa.h
	$(CC) job.h process.h koopa.h -o koopa

clean:
	rm -Rf *.o koopa
