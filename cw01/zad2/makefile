CC = gcc -Wall -std=c11 -ggdb


CC=gcc

all:
	make test
	make clean

clean:
	rm -f *.o *.so *.a

test: library.c main.c stringutils.c fileutils.c
	$(CC) -o test library.c main.c stringutils.c fileutils.c

	rm raport2.txt

	./test create_table 3 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 3 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> raport2.txt;

	./test create_table 3 wc_files 5 ../files/s1.txt ../files/s2.txt ../files/s3.txt ../files/s4.txt ../files/s5.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 3 wc_files 5 ../files/s1.txt ../files/s2.txt ../files/s3.txt ../files/s4.txt ../files/s5.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> raport2.txt;

	./test create_table 3 wc_files 10 ../files/s1.txt ../files/s2.txt ../files/s3.txt ../files/s4.txt ../files/s5.txt ../files/s6.txt ../files/s7.txt ../files/s8.txt ../files/s9.txt ../files/s10.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 3 wc_files 10 ../files/s1.txt ../files/s2.txt ../files/s3.txt ../files/s4.txt ../files/s5.txt ../files/s6.txt ../files/s7.txt ../files/s8.txt ../files/s9.txt ../files/s10.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> raport2.txt;


	./test create_table 100 wc_files 2 ../files/m1.txt ../files/m2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 100 wc_files 2 ../files/m1.txt ../files/m2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> raport2.txt;

	./test create_table 100 wc_files 5 ../files/m1.txt ../files/m2.txt ../files/m3.txt ../files/m4.txt ../files/m5.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 100 wc_files 5 ../files/m1.txt ../files/m2.txt ../files/m3.txt ../files/m4.txt ../files/m5.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> raport2.txt;

	./test create_table 100 wc_files 10 ../files/m1.txt ../files/m2.txt ../files/m3.txt ../files/m4.txt ../files/m5.txt ../files/m6.txt ../files/m7.txt ../files/m8.txt ../files/m9.txt ../files/m10.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 100 wc_files 10 ../files/m1.txt ../files/m2.txt ../files/m3.txt ../files/m4.txt ../files/m5.txt ../files/m6.txt ../files/m7.txt ../files/m8.txt ../files/m9.txt ../files/m10.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> raport2.txt;


	./test create_table 1000 wc_files 2 ../files/l1.txt ../files/l2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 1000 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> raport2.txt;

	./test create_table 1000 wc_files 5 ../files/l1.txt ../files/l2.txt ../files/l3.txt ../files/l4.txt ../files/l5.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 1000 wc_files 5 ../files/l1.txt ../files/l2.txt ../files/l3.txt ../files/l4.txt ../files/l5.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> raport2.txt;

	./test create_table 1000 wc_files 10 ../files/l1.txt ../files/l2.txt ../files/l3.txt ../files/l4.txt ../files/l5.txt ../files/l6.txt ../files/l7.txt ../files/l8.txt ../files/l9.txt ../files/l10.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 1000 wc_files 10 ../files/l1.txt ../files/l2.txt ../files/l3.txt ../files/l4.txt ../files/l5.txt ../files/l6.txt ../files/l7.txt ../files/l8.txt ../files/l9.txt ../files/l10.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> raport2.txt;

	./test create_table 3 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 3 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0  >> raport2.txt;

	rm test
	
	
create_test:
	$(CC) -o test library.c main.c stringutils.c fileutils.c
