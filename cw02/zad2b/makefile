
all:
	make main
	make test

main:
	gcc -Wall main.c -o letter_stats

test:
	./letter_stats a 1out.txt

memory:
	gcc -Wall -o letter_stats main.c -fsanitize=address
	make test
