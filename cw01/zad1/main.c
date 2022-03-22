#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "library.h"
#include "fileutils.h"
#include "stringutils.h"



int main(int argc, char *argv[]) {
    argv++;
    argc--;
    char output_filename[] = "results1.txt";
    char* ptr = calloc(strlen(output_filename), sizeof(char));
    ptr = output_filename;

    struct main_table* main_table = create_main_table(argc);
    wc_files(argv, ptr, argc);

    struct block* block1 = read_file_to_block(output_filename);
    main_table -> blocks[0] = block1;

    delete_main_table(main_table);
}
