//
// Created by debian on 3/18/22.
//
#define MAX_LINE 250
#define MAX_FILENAME 60
#define MAX_INFO 200
#define MAX_MESSAGE 300

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
  fwrite(message, 1, strlen(message), file); \
  sprintf(message, "real time: %f\n", time_in_seconds(clock_start, clock_end));\
  fwrite(message, 1, strlen(message), file);\
  sprintf(message, "sys time: %f\n", time_in_seconds(tms_end.tms_stime, tms_start.tms_stime));\
  fwrite(message, 1, strlen(message), file);\
  sprintf(message, "user time: %f\n", time_in_seconds(tms_end.tms_utime, tms_start.tms_utime));\
  fwrite(message, 1, strlen(message), file);



void wc_task2(char target, char* filename) {
    int target_in_line = 0, counter = 0, line_counter = 0;
    char c[1];
    FILE* input;

    input = fopen(filename, "r");
    if (!input) {
        perror("fopen");
        return;
    }

    while (fread(c, 1, 1, input) == 1) {
        if (c[0] == target) {
            counter++;
            target_in_line = 1;
        }

        if (c[0] == '\n') {
            if (target_in_line == 1) {
                line_counter++;
            }
            target_in_line = 0;
        }
    }

    printf("Occurrences of %c: %d\nLines containing %c: %d\n",target, counter, target, line_counter);

    fclose(input);

}


int main(int argc, char *argv[]) {
    FILE* op;
    op = fopen("pomiar_zad_2b.txt", "a");
    char info[MAX_INFO];

    if (argc == 1) {
        char filename[MAX_FILENAME];
        char target;
        printf("Character: ");
        scanf("%c", &target);
        printf("\nFilename: ");
        scanf("%s", filename);

        sprintf(info, "wc_task2 %c %s", target, filename);
        time_it(info, op, {
            wc_task2(target, filename);
        });

    } else if (argc == 2) {
        char filename[MAX_FILENAME];
        printf("Filename: ");
        scanf("%s", filename);

        sprintf(info, "wc_task2 %c %s", argv[1][0], filename);
        time_it(info, op,
                wc_task2(argv[1][0], filename);
        );

    } else {
        sprintf(info, "wc_task2 %c %s", argv[1][0], argv[2]);
        time_it(info, op,
                wc_task2(argv[1][0], argv[2]);
        );
    }


    fclose(op);
}
