//
// Created by Damian Tworek on 17/03/2022.
//


#define MAX_LINE 250
#define MAX_FILENAME 60
#define MAX_INFO 250
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
  write(file, message, strlen(message)); \
  sprintf(message, "real time: %f\n", time_in_seconds(clock_start, clock_end));\
  write(file, message, strlen(message));\
  sprintf(message, "sys time: %f\n", time_in_seconds(tms_end.tms_stime, tms_start.tms_stime));\
  write(file, message, strlen(message));\
  sprintf(message, "user time: %f\n", time_in_seconds(tms_end.tms_utime, tms_start.tms_utime));\
  write(file, message, strlen(message));


int trunc_file(char* filename, char* output_filename) {
    /*
     * Assumes that each line ends with \n including last one.
     */
    char buff[MAX_LINE], c;
    int input, output, nbytes = -1, p = -1, only_whitespaces = 1;

    input = open(filename, O_RDONLY);

    if (input == -1) {
        printf("Could not open a file.\n");
        return 1;
    }

    output = open(output_filename, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);

    while (read(input, &c, 1) == 1) {
        nbytes++;
        p++;
        buff[p] = c;

        if (isspace(c) == 0) { // 0 means that it is not space
            only_whitespaces = 0;
        }

        if (c == '\n') {
            if (only_whitespaces == 0){  // then write line
                write(output, buff, nbytes);
                write(output, "\n", 1);
            }
            p = -1;
            nbytes = -1;
            only_whitespaces = 1;
        }
    } // eof

    close(input);
    close(output);

    return 0;
}


int main(int argc, char *argv[]) {
    char info[MAX_INFO];
    char time_filename[] = "pomiar_zad_1a.txt";
    int file;
    file = open(time_filename, O_WRONLY | O_APPEND | O_CREAT, 0666);

    if (argc == 1) {
        char filename[MAX_FILENAME];
        char output[MAX_FILENAME];

        printf("Provide filename: ");
        scanf("%s", filename);

        printf("Provide output filename: ");
        scanf("%s", output);

        sprintf(info, "%s %s %s", argv[0], filename, output);

        time_it(info, file, {
            trunc_file(filename, output);
        }
        );
    }
    else if (argc == 2) {
        char output[MAX_FILENAME];

        printf("Provide output filename: ");
        scanf("%s", output);

        sprintf(info, "%s %s %s", argv[0], argv[1], output);

        time_it(info, file,
                trunc_file(argv[1], output);
        );

    } else {
        sprintf(info, "%s %s %s", argv[0], argv[1], argv[2]);
        time_it(info, file,
                trunc_file(argv[1], argv[2]);
        )
    }
    close(file);

}
