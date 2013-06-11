# Compiler
CC = g++

# Compiler flags
CFLAGS = -c

main: intqueue.o queue.h complex.o test.o main.o
	$(CC) intqueue.o queue.h complex.o test.o main.o -o main

main.o: test.h main.cpp
	$(CC) $(CFLAGS) main.cpp

test.o: test.cpp test.h
	$(CC) $(CFLAGS) test.cpp

intqueue.o: intqueue.cpp intqueue.h
	$(CC) $(CFLAGS) intqueue.cpp

complex.o: complex.cpp complex.h
	$(CC) $(CFLAGS) complex.cpp

clean:
	rm -Rf *.o main
