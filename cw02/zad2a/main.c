//
// Created by debian on 3/18/22.
//

#define MAX_FILENAME 60
#define MAX_MESSAGE 300
#define MAX_INFO 200


#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
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



void wc_task2(char target, char* filename) {
    int input, target_in_line = 0, counter = 0, line_counter = 0;
    char c;

    input = open(filename, O_RDONLY);
    if (input == -1) {
        printf("Could not open a file.\n");
        return;
    }


    while (read(input, &c, 1) == 1) {
        if (c == target) {
            counter++;
            target_in_line = 1;
        }

        if (c == '\n') {
            if (target_in_line == 1) {
                line_counter++;
            }
            target_in_line = 0;
        }
    }

    printf("Occurrences of %c: %d\nLines containing %c: %d\n",target, counter, target, line_counter);

    close(input);

}


int main(int argc, char *argv[]) {
    int file;
    file = open("pomiar_zad_2a.txt", O_WRONLY | O_APPEND | O_CREAT, 0666);
    char info[MAX_INFO];



    if (argc == 1) {
        char filename[MAX_FILENAME];
        char target;
        printf("Character: ");
        scanf("%c", &target);
        printf("\nFilename: ");
        scanf("%s", filename);

        sprintf(info, "wc_task2 %c %s", target, filename);

        time_it(info, file,
        wc_task2(target, filename);
        );

    } else if (argc == 2) {
        char filename[MAX_FILENAME];
        printf("Filename: ");
        scanf("%s", filename);
        sprintf(info, "wc_task2 %c %s", argv[1][0], filename);
        time_it(info, file,
                wc_task2(argv[1][0], filename);
        );

    } else {
        sprintf(info, "wc_task2 %c %s", argv[1][0], argv[2]);
        time_it(info, file,
                wc_task2(argv[1][0], argv[2]);
        );
    }
    close(file);
}
