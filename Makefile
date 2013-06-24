# Compiler
CC = g++

# Compiler flags
CFLAGS = -c

koopa: process.h koopa.h koopa.cpp
	$(CC) koopa.cpp -o koopa

clean:
	rm -Rf *.o koopa
