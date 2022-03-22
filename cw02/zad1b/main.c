//
// Created by Damian Tworek on 17/03/2022.
//


#define MAX_LINE 250
#define MAX_FILENAME 60
#define MAX_INFO 200
#define MAX_MESSAGE 300

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "main.h"
#include <sys/times.h>
#include <time.h>

static struct tms tms_start, tms_end;
static clock_t clock_start, clock_end;


double time_in_seconds(clock_t start, clock_t end) {
    return (double)(end - start) / CLOCKS_PER_SEC;
}


#define time_it(name, file, code_block)\
  clock_start = times(&tms_start);\
  code_block\
  clock_end = times(&tms_end);\
  char message[MAX_MESSAGE];\
  sprintf(message, "\n%s\n", name);\
  fwrite(message, 1, strlen(message), file); \
  sprintf(message, "real time: %f\n", time_in_seconds(clock_start, clock_end));\
  fwrite(message, 1, strlen(message), file);\
  sprintf(message, "sys time: %f\n", time_in_seconds(tms_end.tms_stime, tms_start.tms_stime));\
  fwrite(message, 1, strlen(message), file);\
  sprintf(message, "user time: %f\n", time_in_seconds(tms_end.tms_utime, tms_start.tms_utime));\
  fwrite(message, 1, strlen(message), file);



int trunc_file(char* filename, char* output_filename) {
    /*
     * Assumes that each line ends with \n including last one.
     */
    char buff[MAX_LINE], c[1];
    int nbytes = -1, p = -1, only_whitespaces = 1;
    FILE* input;
    FILE* output;

    input = fopen(filename, "r");
    if (!input) {
        perror("fopen");
        return 1;
    }


    output = fopen(output_filename, "w");

    while (fread(c, 1, 1, input) == 1) {
        nbytes++;
        p++;
        buff[p] = c[0];

        if (isspace(c[0]) == 0) { // 0 means that it is not space
            only_whitespaces = 0;
        }

        if (c[0] == '\n') {
            if (only_whitespaces == 0){  // then write line
                fwrite(buff, 1, nbytes, output);
                fwrite("\n", 1, 1, output);
            }
            p = -1;
            nbytes = -1;
            only_whitespaces = 1;
        }
    } // eof

    fclose(input);
    fclose(output);

    return 0;
}


int main(int argc, char *argv[]) {
    char info[MAX_INFO];
    char time_filename[] = "pomiar_zad_1b.txt";
    FILE* time_file;

    time_file = fopen(time_filename, "a");

    if (argc == 1) {
        char filename[MAX_FILENAME];
        char output[MAX_FILENAME];

        printf("Provide filename: ");
        scanf("%s", filename);

        printf("Provide output filename: ");
        scanf("%s", output);

        sprintf(info, "%s %s %s", argv[0], filename, output);

        time_it(info, time_file, {
            trunc_file(filename, output);
        }
        );
    }
    else if (argc == 2) {
        char output[MAX_FILENAME];

        printf("Provide output filename: ");
        scanf("%s", output);
        sprintf(info, "%s %s %s", argv[0], argv[1], output);

        time_it(info, time_file,
            trunc_file(argv[1], output);
        );


    } else {
        sprintf(info, "%s %s %s", argv[0], argv[1], argv[2]);
        time_it(info, time_file,
            trunc_file(argv[1], argv[2]);
        )
    }

    fclose(time_file);
}
