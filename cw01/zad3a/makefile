CC = gcc -Wall


output = raport3a.txt

all:
	make static
	make shared
	make dynamic
	test
	clean

clean:
	rm -f *.o *.so *.a

static:
	make clear

	$(CC) -c library.c main.c
	ar rcs library.a library.o

shared: library.c main.c
	$(CC) -c -fPIC library.c
	$(CC) -shared -fPIC -o library.so library.o


dynamic: main_dynamic.c shared
	$(CC) -o main main_dynamic.c -L. -ldl -D DYNAMIC

test:
	make tests-static
	make tests-shared
	make tests_dynamic

tests-static: library.c main.c
	make static
	$(CC) -o test main.c library.a -I .

	echo "\nStatic library\n" >> $(output)

	make tests

	rm test

tests-shared: library.c main.c
	make shared
	$(CC) -o test main.c -L. library.so -I .


	echo "\nShared library\n" >> $(output)

	make tests

	rm test


tests_dynamic:
	make shared
	$(CC) -o test main_dynamic.c -I .

	echo "\nDynamic library\n" >> $(output)

	make tests

	rm test


create_test:
	$(CC) -o test library.c main.c


clear:
	rm -f *.o *.a *.so main

tests:
	./test create_table 3 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 3 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> $(output);

	./test create_table 3 wc_files 5 ../files/s1.txt ../files/s2.txt ../files/s3.txt ../files/s4.txt ../files/s5.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 3 wc_files 5 ../files/s1.txt ../files/s2.txt ../files/s3.txt ../files/s4.txt ../files/s5.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> $(output);

	./test create_table 3 wc_files 10 ../files/s1.txt ../files/s2.txt ../files/s3.txt ../files/s4.txt ../files/s5.txt ../files/s6.txt ../files/s7.txt ../files/s8.txt ../files/s9.txt ../files/s10.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 3 wc_files 10 ../files/s1.txt ../files/s2.txt ../files/s3.txt ../files/s4.txt ../files/s5.txt ../files/s6.txt ../files/s7.txt ../files/s8.txt ../files/s9.txt ../files/s10.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> $(output);


	./test create_table 100 wc_files 2 ../files/m1.txt ../files/m2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 100 wc_files 2 ../files/m1.txt ../files/m2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> $(output);

	./test create_table 100 wc_files 5 ../files/m1.txt ../files/m2.txt ../files/m3.txt ../files/m4.txt ../files/m5.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 100 wc_files 5 ../files/m1.txt ../files/m2.txt ../files/m3.txt ../files/m4.txt ../files/m5.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> $(output);

	./test create_table 100 wc_files 10 ../files/m1.txt ../files/m2.txt ../files/m3.txt ../files/m4.txt ../files/m5.txt ../files/m6.txt ../files/m7.txt ../files/m8.txt ../files/m9.txt ../files/m10.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 100 wc_files 10 ../files/m1.txt ../files/m2.txt ../files/m3.txt ../files/m4.txt ../files/m5.txt ../files/m6.txt ../files/m7.txt ../files/m8.txt ../files/m9.txt ../files/m10.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> $(output);


	./test create_table 1000 wc_files 2 ../files/l1.txt ../files/l2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 1000 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> $(output);

	./test create_table 1000 wc_files 5 ../files/l1.txt ../files/l2.txt ../files/l3.txt ../files/l4.txt ../files/l5.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 1000 wc_files 5 ../files/l1.txt ../files/l2.txt ../files/l3.txt ../files/l4.txt ../files/l5.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> $(output);

	./test create_table 1000 wc_files 10 ../files/l1.txt ../files/l2.txt ../files/l3.txt ../files/l4.txt ../files/l5.txt ../files/l6.txt ../files/l7.txt ../files/l8.txt ../files/l9.txt ../files/l10.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 1000 wc_files 10 ../files/l1.txt ../files/l2.txt ../files/l3.txt ../files/l4.txt ../files/l5.txt ../files/l6.txt ../files/l7.txt ../files/l8.txt ../files/l9.txt ../files/l10.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 >> $(output);

	./test create_table 3 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0
	./test create_table 3 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0 wc_files 2 ../files/s1.txt ../files/s2.txt wc_output.txt read_file wc_output.txt 0 remove_block 0  >> $(output);



