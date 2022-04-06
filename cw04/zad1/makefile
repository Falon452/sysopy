
all:
	rm main
	make main
	make test

main:
	gcc -Wall -fsanitize=address main.c -o main

test:
#	./main ignore fork
	echo "\n"
#	./main handler fork
	echo "\n"
#	./main mask fork
	echo "\n"
	#./main pending fork
	echo "\n"
	./main ignore exec
	echo "\n"
	./main mask exec
	echo "\n"
	./main pending exec
	echo "\n"