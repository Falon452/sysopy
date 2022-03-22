all:
	make main
	make test

main:
	gcc -Wall main.c -o my_ls

test:
	./my_ls ../z3-dir

memory:
	gcc -Wall -o my_ls main.c -fsanitize=address
	make test
