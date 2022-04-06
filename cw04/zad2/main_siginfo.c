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



void handle_SIGILL(int signum, siginfo_t *info, void *context) {
    printf("Signal number %d\n", info -> si_signo);
    printf("Memory location that caused fault %p\n", info -> si_addr);
    exit(1); // because it returns to the same value making infinite loop
}


void handle_SIGINT(int signum, siginfo_t *info, void *context) {
    printf("Signal number %d\n", info->si_signo);
    printf("Sending PID %d\n", info->si_pid);
    exit(1);

}

//void handle_SIGSEGV(int signum, siginfo_t *info, void *context) {
//    printf("Signal number %d\n", info -> si_signo);
//    printf("Memory location that caused fault %p\n", info -> si_addr);
//    exit(1); // because it returns to the same value making infinite loop
//}



//void handle_SIGFPE(int signum, siginfo_t *info, void *context) {
//    printf("Signal number %d\n", info -> si_signo);
//    printf("Signal number %d\n", info -> si_signo);
//    printf("Sending process ID %d\n", info -> si_pid);
//    printf("Real user ID = %d\n",info -> si_uid);
//    printf("Memory location that caused fault %p\n", info -> si_addr);
//}


void handle_SIGCHLD(int signum, siginfo_t *info, void *context) {
    printf("Signal number %d\n", info -> si_signo);
    printf("Sending process ID %d\n", info -> si_pid);
    printf("Real user ID of sending %d\n", info -> si_uid);
    printf("Exit value or sigal %d\n", info -> si_status);
}


int main(int argc,char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s SIGILL|SIGCHLD|SIGINT", argv[0]);
        exit(1);
    }

    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO|SA_NODEFER | SA_NOMASK;

    if (strcmp(argv[1], "SIGILL") == 0) {
        action.sa_sigaction = handle_SIGILL;
        sigaction(SIGILL, &action, NULL);

        asm(".word 0x00000000");  // illegal assembler instruction
    }

    if (strcmp(argv[1], "SIGCHLD") == 0) {
        action.sa_sigaction = handle_SIGCHLD;
        sigaction(SIGCHLD, &action, NULL);
        if (fork() == 0) {
            exit(1);
        }
    }

    if (strcmp(argv[1], "SIGINT") == 0) { // ctrl + C
        action.sa_sigaction = handle_SIGINT;
        sigaction(SIGINT, &action, NULL);

        printf("waiting for ctrl+C...\n");
        while(1);
    }



    // no floating point error here no idea why
//    if (strcmp(argv[1], "SIGFPE") == 0) {
//        action.sa_sigaction = handle_SIGFPE;
//        sigaction(SIGFPE, &action, NULL);
//        raise(SIGFPE);
//
//        int res, x = 0;
//        res = 7 / x;
//        printf("%d\n", res);
//    }


    // not working
//    if (strcmp(argv[1], "SIGSEGV") == 0) {
//        action.sa_sigaction = handle_SIGSEGV;
//        sigaction(SIGSEGV, &action, NULL);
//        printf("In segsgv\n");
//        int A[10];
//        for (int i = 0; i <= 10; i++) {
//            A[i] = i;
//        }
//    }

}