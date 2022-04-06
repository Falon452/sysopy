#define _POSIX_C_SOURCE 1
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>


void check_pending() {
    sigset_t pending;
    sigpending(&pending);

    if (sigismember(&pending, SIGUSR1)){
        printf("Signal %d is pending\n", SIGUSR1);
    } else {
        printf("Signal %d is not pending\n", SIGUSR1);
    }
}

int main(int argc, char* argv[]) {
    printf("exec ");
    if (strcmp(argv[1], "pending") == 0) {
        raise(SIGUSR1);
    }

    if (strcmp(argv[1], "ignore") == 0) {
        struct sigaction action;
        sigaction(SIGUSR1, NULL, &action);
        if (action.sa_handler == SIG_IGN) {
            printf("signal is ignored\n");
        } else {
            printf("signal is not ignored\n")
        }
    }
    if (strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending")) {
        check_pending();
    }

}