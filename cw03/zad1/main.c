//
// Created by debian on 3/26/22.
//
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s number\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);
    pid_t child_pid;

    for (int i = 0; i < n; i++ ) {
        if ((child_pid = fork()) == 0 ) {
            printf("Proces dziecka: %d. Proces rodzica: %d.\n", (int)getpid(), (int)getppid());
            exit(0);
        }
    }


}