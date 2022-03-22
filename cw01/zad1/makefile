CC = gcc -Wall -std=c11 -ggdb


CC=gcc
CFLAGS=-I.

all:
	make wc_files
	make shared
	make clean

wc_files: library.c main.c stringutils.c fileutils.c
	$(CC) library.c main.c stringutils.c fileutils.c -o wc_files

shared: library.c main.c
	$(CC) -c -fPIC library.c stringutils.c fileutils.c
	$(CC) -shared -fPIC -o library.so library.o stringutils.o fileutils.o

all:

clean:
	rm -f *.o *.so *.a
