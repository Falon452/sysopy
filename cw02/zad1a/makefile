

all:
	make main
	make test

test:
	./del_empty_lines ../z1-txt/1.txt 1out.txt
	./del_empty_lines ../z1-txt/2.txt 2out.txt
	./del_empty_lines ../z1-txt/3.txt 3out.txt

	./del_empty_lines ../z1-txt/4.txt 4out.txt
	./del_empty_lines ../z1-txt/5.txt 5out.txt

main:
	gcc -Wall -o del_empty_lines main.c


memory:
	gcc -Wall -o del_empty_lines main.c -fsanitize=address
	make test



