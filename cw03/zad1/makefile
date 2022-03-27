
all:
	rm main
	make main
	make test

main:
	gcc -Wall -fsanitize=address main.c -o main

test:
	./main 40