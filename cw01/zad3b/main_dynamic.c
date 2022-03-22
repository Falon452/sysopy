//
// Created by Damian Tworek on 13/03/2022.
//

//
// Created by Damian Tworek on 13/03/2022.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "library.h"
#include <sys/times.h>
#include <time.h>
#include <dlfcn.h>

static struct tms tms_start, tms_end;
static clock_t clock_start, clock_end;


double time_in_seconds(clock_t start, clock_t end) {
    return (double)(end - start) / CLOCKS_PER_SEC;
}


#define time_it(name, code_block)\
  clock_start = times(&tms_start);\
  code_block\
  clock_end = times(&tms_end);\
  printf("\n%s\n", name);\
  printf("real time: %f\n", time_in_seconds(clock_start, clock_end));\
  printf(" sys time: %f\n", time_in_seconds(tms_end.tms_stime, tms_start.tms_stime));\
  printf("user time: %f\n", time_in_seconds(tms_end.tms_utime, tms_start.tms_utime));



int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Not enough arguments.\n");
        return 0;
    }
    argv++; argc--;
    if (strcmp(argv[0], "create_table") != 0) {
        printf("First argument must be create_table.\n");
        exit(1);
    }

    void *handle = dlopen("./library.so", RTLD_LAZY);
    if(!handle){
        printf("dlopen returned error: %s\n", dlerror());
        exit(1);
    }

    clock_start = times(&tms_start);

    int number_of_blocks = atoi(argv[1]);

    struct main_table* (*fcreate_main_table)();
    fcreate_main_table = (struct main_table* (*)())dlsym(handle,"create_main_table");

    if(dlerror() != NULL){
        printf("dlsym returned error: %s", dlerror());
        exit(1);
    }

    struct main_table* main_table = (*fcreate_main_table)(number_of_blocks);

    clock_end = times(&tms_end);
    printf("\ncreate_table %d\n", number_of_blocks);
    printf("real time: %f\n", time_in_seconds(clock_start, clock_end));
    printf(" sys time: %f\n", time_in_seconds(tms_end.tms_stime, tms_start.tms_stime));
    printf("user time: %f\n", time_in_seconds(tms_end.tms_utime, tms_start.tms_utime));

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "wc_files") == 0) {
            char message[36];

            i++;
            int number_of_files = atoi(argv[i]);
            i++;
            char** files = calloc(number_of_files, sizeof(char*));


            for (int k = 0; k < number_of_files; i++, k++) {
                files[k] = argv[i];
            }
            char type[7];

            if (files[0][9] == 's') {
                type[0]='s'; type[1]='m';type[2]='a';type[3]='l';type[4]='l';type[5]='\0';  // xd
            }
            if (files[0][9] == 'm') {
                type[0]='m'; type[1]='e';type[2]='d';type[3]='i';type[4]='u';type[5]='m';type[6]='\0'; // xd
            }
            if (files[0][9] == 'l') {
                type[0]='l'; type[1]='a';type[2]='r';type[3]='g';type[4]='e';type[5]='\0'; // xd
            }


            sprintf(message, "wc_files %s %d", type, number_of_files);
            time_it(message, {
                void (*fwc_files)();
                fwc_files = (void (*)())dlsym(handle,"wc_files");
                if(dlerror() != NULL){
                    printf("dlsym returned error: %s", dlerror());
                    exit(1);
                }
                (*fwc_files)(files, argv[i], number_of_files);
            });

            free(files);
            continue;
        }

        if (strcmp(argv[i], "read_file") == 0) {
            char message[36];
            i++;
            char* filename = argv[i];
            i++; // block_index
            int index = atoi(argv[i]);
            sprintf(message, "read_file %s ", filename);

            time_it(message, {
                struct block* (*fread_file_to_block)();
                fread_file_to_block = (struct block* (*)())dlsym(handle,"read_file_to_block");
                if(dlerror() != NULL){
                    printf("dlsym returned error: %s", dlerror());
                    exit(1);
                }
                struct block* block = (*fread_file_to_block)(filename);
                main_table -> blocks[index] = block;
            });

            continue;
        }

        if (strcmp(argv[i], "remove_block") == 0) {
            char message[36];
            i++;
            int index = atoi(argv[i]);
            sprintf(message, "remove_block of index: %d ", index);
            time_it(message, {
                void (*ffree_block)();
                ffree_block = (void (*)())dlsym(handle,"free_block");
                (*ffree_block)(main_table -> blocks[index]);
                main_table -> blocks[index] = NULL;
            });
            continue;
        }
        printf("Invalid argument: %s.\n", argv[i]);
        exit(1);
    }
    void (*delete_main_table)();
    delete_main_table = (void (*)())dlsym(handle,"delete_main_table");

    if(dlerror() != NULL){
        printf("dlsym returned error: %s", dlerror());
        exit(1);
    }
    (*delete_main_table)(main_table);
}