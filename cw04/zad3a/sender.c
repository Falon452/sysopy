//
// Created by debian on 4/5/22.
//
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

char type[20];


int SIGUSR1_counter = 0;
int standard_sig = SIGUSR1;
int ending_sig = SIGUSR2;


void handler(int signum) {
    if (signum == standard_sig) {
        printf("Caught SIGUSR1\n");
        SIGUSR1_counter++;
    }
    if (signum == ending_sig) {
        printf("Caught SIGUSR2\n");
        printf("I got back %d signals\n", SIGUSR1_counter);
        exit(0);
    }

}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s PID number-of-signals KILL|SIGQUEUE|SIGRT\n", argv[0]);
        exit(1);
    }
    printf("My PID %d", getpid());
    int nof_signals = atoi(argv[2]);
    if (nof_signals < 1) {
        printf("pls send some signals\n");
        exit(1);
    }

    type[sizeof(type) - 1] = '\0';
    if (strcmp(argv[3], "KILL") == 0) {
        strncpy(type, argv[3], sizeof(type) - 1);
    } else if (strcmp(argv[3], "SIGQUEUE") == 0) {
        strncpy(type, argv[3], sizeof(type) - 1);
    } else if (strcmp(argv[3], "SIGRT") == 0) {
        standard_sig = SIGRTMIN;
        ending_sig = SIGRTMIN+1;
        strncpy(type, "KILL", sizeof(type) - 1);
    } else {
        printf("invalid type\n");
        exit(1);
    }

    pid_t catcher_pid = atoi(argv[1]);
    printf("Catcher pid: %d\n", catcher_pid);

    sigset_t sigset;
    sigfillset(&sigset); // fills all available signals
    sigdelset(&sigset, standard_sig);
    sigdelset(&sigset, ending_sig);

    struct sigaction action;
    action.sa_flags = SA_NODEFER;  // enable receiving signals inside handler
    action.sa_handler = handler;

    if (sigaction(standard_sig, &action, NULL) != 0) {  // unblock sigusr1
        printf("sigaction error SIGUSR1\n");
        exit(1);
    }
    if (sigaction(ending_sig, &action, NULL) != 0) { // unblock sigusr2
        printf("Sigaction SIGUSR2 error\n");
        exit(1);
    }

    if (strcmp(type, "KILL") == 0) {
        for (int i = 0; i < nof_signals; i++) {
            printf("Sending SIGUSR1 to catcher\n");
            if (kill(catcher_pid, standard_sig) != 0)
                printf("Something went wrong with kill()! %s\n", strerror(errno));
        }

        sleep(2);

        printf("Sending SIGUSR2 to catcher\n");
        if (kill(catcher_pid, ending_sig) != 0)
            printf("Something went wrong with kill()! %s\n", strerror(errno));
    }


    if (strcmp(type, "SIGQUEUE") == 0) {
        union sigval qval;
        qval.sival_int = 1;
        for (int i =0; i < nof_signals; i++) {
            sigqueue(catcher_pid, standard_sig, qval);
            qval.sival_int++;
        }

        sleep(2);
        sigqueue(catcher_pid, ending_sig, qval);
        qval.sival_int++;
    }

    if (strcmp(type, "SIGRT") == 0) {

    }

    printf("I am waiting now waiting for signals!\n");
    while (1) {
        if (sigsuspend(&sigset) == -1)// replaces current mask with &sigset
            printf("Sigsuspend returned\n");
    }
    //continue

}

