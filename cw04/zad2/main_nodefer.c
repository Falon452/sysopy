//
// Created by debian on 4/5/22.
//
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
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

void handler(int signo)
{
    printf("\nInto handler\n");
    while(1);
}

void check_pending(){
    sigset_t pending;
    sigpending(&pending);

    if (sigismember(&pending, SIGUSR1)) {
        printf("Signal %d is pending\n", SIGUSR1);
    } else {
        printf("Signal %d is not pending\n", SIGUSR1);
    }
}

int main(int argc, char* argv[]) {
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);

    // This flag allows catching signal inside of handler ;)
    action.sa_flags = SA_NODEFER;
    sigaction(SIGINT, &action, NULL);

    printf("Im main...\n");
    while(1);
}

