//
// Created by Damian Tworek on 11/03/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "library.h"
#include "fileutils.h"
#include "stringutils.h"


#define MAX_FILENAME 100


struct main_table* create_main_table(int number_of_blocks) {
    if (number_of_blocks <= 0) {
        printf("WARNING: Can't create main table, number of blocks must be greater than 0.\n");
        return NULL;
    }

    struct main_table* main_table = calloc(1, sizeof(struct main_table));
    if (main_table == NULL) {
        printf("WARNING: main_table is NULL after calloc. Probably no memory.");
        return NULL;
    }

    main_table -> number_of_blocks = number_of_blocks;
    main_table -> blocks = calloc(number_of_blocks, sizeof(struct block*));
    if (main_table -> blocks == NULL) {
        printf("WARNING: main_table is NULL after calloc. Probably no memory.");
        return NULL;
    }
    for (int i = 0; i < number_of_blocks; i++) {
        main_table -> blocks[i] = NULL;
    }

    return main_table;
}


struct block* create_block(int number_of_files) {
    if (number_of_files <= 0) {
        printf("WARNING: number_of_files must be > 0");
        return NULL;
    }

    struct block* block = calloc(1, sizeof(struct block));
    if (block == NULL) {
        printf("WARNING: block is NULL after calloc. Probably no memory.");
        return NULL;
    }

    block -> number_of_files = number_of_files;

    block -> wc_files = calloc(number_of_files, sizeof(struct wc_file*));
    if (block -> wc_files == NULL) {
        printf("WARNING: block -> wc_files is NULL after calloc. Probably no memory.");
    }
    for (int i = 0; i < number_of_files; i++ ) {
        block -> wc_files[i] = NULL;
    }

    return block;
}


struct wc_file* create_wc_file(int lines, int words, int chars, char* filename) {
    if (lines < 0 || words < 0 || chars < 0){
        printf("WARNING: Can't create a block, lines or words, or chars are less than 0.\n");
        return NULL;
    }

    struct wc_file* wc_file = calloc(1, sizeof(struct wc_file));
    if (wc_file == NULL) {
        printf("WARNING: wc_file is NULL after calloc. Probably no memory.");
        return NULL;
    }

    wc_file -> lines = lines;
    wc_file -> words = words;
    wc_file -> chars = chars;

    int filename_len = strlen(filename);

    char* ptr = calloc(filename_len + 1, sizeof(char));
    if (ptr == NULL){
        printf("WARNING: ptr is NULL after calloc. Probably no memory.");
        return NULL;
    }

    wc_file -> filename = ptr;

    int i = 0;
    for (; i < filename_len; i++) {
        ptr[i] = filename[i];
    }
    ptr[i] = '\0';

    return wc_file;
}


void free_wc_file(struct wc_file* wc_file) {
    if (wc_file == NULL)
        return;
    free(wc_file -> filename);
    wc_file -> filename = NULL;

    free(wc_file);
    wc_file = NULL;
}


void delete_wc_file_at(struct block* block, int index) {
    if (block == NULL) {
        return;
    }
    if (block -> wc_files[index] == NULL) {
        return;
    }
    free_wc_file(block -> wc_files[index]);
}


void delete_block_at(struct main_table* main_table, int index) {
    if (index < 0){
        printf("WARNING: Index must be >= 0.\n");
        return;
    }
    if (main_table == NULL) {
        printf("WARNING: Main table is NULL.\n");
        return;
    }
    if (main_table -> blocks[index] == NULL) {
        return;
    }
    if (index >= main_table -> number_of_blocks || index < 0){
        printf("WARNING: Invalid index value %d, indexes are from 0 to %d.\n", index, main_table -> number_of_blocks);
        return;
    }

    int n = main_table -> blocks[index] -> number_of_files;

    for (int i = 0; i < n; i++) {
        if (main_table -> blocks[index] -> wc_files[i] == NULL)
            continue;

        delete_wc_file_at(main_table -> blocks[index], i);
    }

    free(main_table -> blocks[index]);
    main_table -> blocks[index] = NULL;
}


void free_block(struct block* block) {
    if (block == NULL)
        return;
    for (int i = 0; i < block -> number_of_files; i++ ) {
        delete_wc_file_at(block, i);
        block -> wc_files[i] = NULL;
    }
    free(block);
    block = NULL;
}


void delete_main_table(struct main_table* main_table){
    if (main_table == NULL) {
        return;
    }
    for (int i = 0; i < main_table -> number_of_blocks; i++) {
        delete_block_at(main_table, i);
    }
    free(main_table);
    main_table = NULL;
}


void wc_files(char** files, char* output_filename, int number_of_files) {
    for (int i = 0; i < number_of_files; i++) {
        if (access(files[i], R_OK) == 0) {
            continue;
        } else {
            printf("ERROR: File: %s doesn't exists or can't be read.\n", files[i]);
            return;
        }
    }

    int len_flattened = number_of_chars_in_array(files, number_of_files);
    int len_output_filename = str_len(output_filename);
    char* flattened_string = calloc(len_flattened + number_of_files, sizeof(char));
    char* wc_command = calloc(strlen("wc ") + len_flattened + number_of_files - 1 + strlen(" > ") + len_output_filename + 1, 1);

    flatten_array_of_strings(files, flattened_string, number_of_files);

    wc_command[0] = 'w'; wc_command[1] = 'c'; wc_command[2] = ' ';

    int  i = 3;
    for (int j = 0; flattened_string[j] != '\0'; i++, j++) {
        wc_command[i] = flattened_string[j];
    }
    wc_command[i++] = ' '; wc_command[i++] = '>'; wc_command[i++] = ' ';


    for (int ix = 0; output_filename[ix] != '\0'; ix++, i++){
        wc_command[i] = output_filename[ix];
    }
    wc_command[i] = '\0';


    system(wc_command);

    free(flattened_string);
    free(wc_command);
}


struct block* read_file_to_block(char* filename) {

    FILE* fp;

    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("WARNING: Could not open file %s\n", filename);
        return NULL;
    }


    int nof_lines = count_lines(filename);
    struct block* block = create_block(nof_lines);
    int lines, words, chars;
    char f[MAX_FILENAME];

    for (int i =0; i < nof_lines - 1; i++) {  // -1 because wc makes one additional newline
        int number_of_matched = fscanf(fp, "%d %d %d %s", &lines, &words, &chars, f);
        if (number_of_matched != 4){
            printf("WARNING: Line number: %d did not match, breaking\n", i+1);
            break;
        }
        block -> wc_files[i] = create_wc_file(lines, words, chars, f);
    }

    fclose(fp);

    return block;
}


