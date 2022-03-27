//
// Created by debian on 3/24/22.
//

#define MAX_FILENAME 50
#define MAX_MESSAGE 100

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include<sys/wait.h>
#include <string.h>

int N = 1;
int status = 0;


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
  sprintf(message, "sys time: %f\n", time_in_seconds(tms_start.tms_stime, tms_end.tms_stime));\
  fwrite(message, 1, strlen(message), file);\
  sprintf(message, "user time: %f\n", time_in_seconds(tms_start.tms_utime, tms_end.tms_utime));\
  fwrite(message, 1, strlen(message), file); \
  fclose(file);

double f(double x) {
    return 4/(x * x + 1);
}

double one_rectangle(double dx, double fx) {
    return dx * fx;
}


int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s number\n", argv[0]);
        exit(1);
    }
    FILE *measure_file;
    measure_file = fopen("pomiar-zad2.txt", "a");

    int n = atoi(argv[1]);
    double dx = 1 / (double)n;
    double x = dx;
    pid_t wpid;

    char time_it_msg[30];
    sprintf(time_it_msg, "for n = %d", n);

    time_it(time_it_msg, measure_file, {
        for (int i = 0; i < n; i++) {
            if (vfork() == 0) {
                FILE *fp;
                char filename[MAX_FILENAME];

                sprintf(filename, "w%d.txt", N);
                fp = fopen(filename, "w");
                fprintf(fp, "%f", one_rectangle(dx, f(x)));

                fclose(fp);
                N++;
                x += dx;
                exit(0);
            }
        }

        while ((wpid = wait(&status)) > 0);

        FILE *fpp;

        double result = 0;
        char fname[MAX_FILENAME];

        for (int i = 1; i < n + 1; i++) {
            double partial;
            sprintf(fname, "w%d.txt", i);
            fpp = fopen(fname, "r");
            fscanf(fpp, "%lf", &partial);
            result += partial;
            fclose(fpp);
        }

        printf("%f\n", result);
    }
    ); // end of time it
}