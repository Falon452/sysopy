//
// Created by Damian Tworek on 11/03/2022.
//
#pragma once


struct main_table{
    int number_of_blocks;
    struct block** blocks;
};


struct block {
    int number_of_files;
    struct wc_file** wc_files;
};

struct wc_file {
    int lines;
    int words;
    int chars;
    char* filename;
};



void wc_files(char** files, char* output_filename, int number_of_files);

struct main_table* create_main_table(int number_of_blocks);

struct block* create_block(int number_of_files);

struct wc_file* create_wc_file(int lines, int words, int chars, char* filename);

void delete_wc_file_at(struct block* block, int index);

void free_wc_file(struct wc_file* wc_file);

void delete_block_at(struct main_table* main_table, int index);

void free_block(struct block* block);

void delete_main_table(struct main_table* main_table);

struct block* read_file_to_block(char* filename);





