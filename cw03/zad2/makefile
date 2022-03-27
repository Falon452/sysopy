
all:
	rm main
	make main
	make test

rmmain:
	rm main

main:
	gcc -Wall -fsanitize=address main.c -o main

test:
	./main 10
	./main 100
	./main 1000
	./main 8000
	rm *.txt

clean:
	rm *.txt