
all:
	rm main
	make main
	make test

rmmain:
	rm main

main:
	gcc -Wall -fsanitize=address main.c -o main

test:
	./main ./z3-dir bcdb 1

clean:
	rm *.txt