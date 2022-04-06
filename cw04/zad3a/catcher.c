//
// Created by debian on 4/5/22.
//
#include <fenv.h>
#include <limits.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/ucontext.h>

int SIGUSR1_counter = 0;
pid_t sender_pid = -1;
char type[20];

int standard_sig = SIGUSR1;
int ending_sig = SIGUSR2;

void catcher(int signum, siginfo_t *info, void *context) {
    if (signum == standard_sig) {
        if (strcmp(type, "SIGQUEUE") == 0) {
            union sigval qval = info -> si_value;
            printf("number from sender %d \n", qval.sival_int);
        }
        printf("catcher caught SIGUSR1\n");
        SIGUSR1_counter++;
    }

    if (signum == ending_sig) {
        printf("catcher caught SIGUSR2!\n");
        printf("sending process id: %d\n", info -> si_pid);
        sender_pid = info -> si_pid;
        printf("I've caught %d signals and im sending it back..\n", SIGUSR1_counter);

        if (strcmp(type, "KILL") == 0) {
            for (int i = 0; i < SIGUSR1_counter; i++)
                kill(sender_pid, standard_sig);
            printf("Sending SIGUSR2\n");
            kill(sender_pid, ending_sig);
        }

        if (strcmp(type, "SIGQUEUE") == 0) {
            union sigval qval;
            qval.sival_int = 1;
            for (int i = 0; i < SIGUSR1_counter; i++) {
                sigqueue(sender_pid, standard_sig, qval);
                qval.sival_int++;
            }

            printf("Sending SIGUSR2\n");
            sigqueue(sender_pid, ending_sig, qval);
        }
        exit(0);
    }
}
//void catcher(int signum, siginfo_t *info, void *context) {
//    switch (signum) {
//        case standard_sig:
//            if (strcmp(type, "SIGQUEUE") == 0) {
//                union sigval qval = info -> si_value;
//                printf("number from sender %d \n", qval.sival_int);
//            }
//            printf("catcher caught SIGUSR1\n");
//            SIGUSR1_counter++;
//            break;
//        case ending_sig:
//            printf("catcher caught SIGUSR2!\n");
//            printf("sending process id: %d\n", info -> si_pid);
//            sender_pid = info -> si_pid;
//            printf("I've caught %d signals and im sending it back..\n", SIGUSR1_counter);
//
//            if (strcmp(type, "KILL") == 0) {
//                for (int i = 0; i < SIGUSR1_counter; i++)
//                    kill(sender_pid, standard_sig);
//                printf("Sending SIGUSR2\n");
//                kill(sender_pid, ending_sig);
//            }
//
//            if (strcmp(type, "SIGQUEUE") == 0) {
//                union sigval qval;
//                qval.sival_int = 1;
//                for (int i = 0; i < SIGUSR1_counter; i++) {
//                    sigqueue(sender_pid, standard_sig, qval);
//                    qval.sival_int++;
//                }
//
//                printf("Sending SIGUSR2\n");
//                sigqueue(sender_pid, ending_sig, qval);
//            }
//            exit(0);
//        default:
//            printf("catcher caught unexpected signal %d\n", signum);
//    }
//}

int main(int argc, char* argv[]) {
    if (argc != 2 ) {
        printf("Usage: %s KILL|SIGQUEUE|SIGRT\n", argv[0]);
        exit(1);
    }

    type[sizeof(type) - 1] = '\0';
    if (strcmp(argv[1], "KILL") == 0) {
        strncpy(type, argv[1], sizeof(type) - 1);
    } else if (strcmp(argv[1], "SIGQUEUE") == 0) {
        strncpy(type, argv[1], sizeof(type) - 1);
    } else if (strcmp(argv[1], "SIGRT") == 0) {
        standard_sig = SIGRTMIN;
        ending_sig = SIGRTMIN+1;
        strncpy(type, "KILL", sizeof(type) - 1);
    } else {
        printf("invalid type\n");
        exit(1);
    }

    int pid;
    pid = getpid();
    printf("Catcher pid: %d\n", pid);

    sigset_t sigset;
    sigfillset(&sigset); // fills all available signals
    sigdelset(&sigset, standard_sig); // deletes sigusr1
    sigdelset(&sigset, ending_sig); // deletes sigusr2

    struct sigaction action;

    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO| SA_NODEFER; // to retrieve sender PID
    action.sa_sigaction = catcher;

    if (sigaction(standard_sig, &action, NULL) != 0)
        printf("sigaction SIGUSR1 error\n");
    else if (sigaction(ending_sig, &action, NULL) != 0)
        printf("sigaction SIGUSR2 error\n");
    else {
        printf("Waiting for SIGUSR1\n");
        while (1) {
            if (sigsuspend(&sigset) == -1)  // suspend all in the set (except SIGUSR1), and wait for any not suspended
                printf("Sigsuspend returned - got signal..\n");
            else
                printf("Something wrong with sigsuspend\n");
        }
    }

}
