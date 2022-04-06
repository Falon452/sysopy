#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <unistd.h>
#include <errno.h>

// handle the signal from it's normal usage
void handler(int signum) {
    printf("handler: received signal %d\n", signum);
}

// this is adds SIGUSR1 to mask - set of signals that are blocked from delivery to a process
void mask_signal() {
    sigset_t new_mask;
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &new_mask, NULL) < 0) {
        fprintf(stderr, "Signal could not be blocked: %s\n", strerror(errno));
        exit(1);
    }
}

// checks currently pending(blocked) signals
void check_pending() {
    sigset_t pending;
    sigpending(&pending);

    if (sigismember(&pending, SIGUSR1)) {
        printf("Signal %d is pending\n", SIGUSR1);
    } else {
        printf("Signal %d is not pending\n", SIGUSR1);
    }
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s ignore|handler|mask|pending fork|exec", argv[0]);
        exit(1);
    }

    if (!strcmp(argv[1], "ignore")) {
        signal(SIGUSR1, SIG_IGN); // ignore SIGUSR1 if delivered
        // signal() is not recommended, better use sigaction
    } else if (!strcmp(argv[1], "handler")) {
        signal(SIGUSR1, handler); // change deafult action to handler void (* handler)(int)
    } else if (!strcmp(argv[1], "mask") || !strcmp(argv[1], "pending")) {
        mask_signal(); // adds SIGUSR1 to be blocked in mask
    }

    printf("Raising signal in main...\n");
    raise(SIGUSR1);

    if (!strcmp(argv[1], "pending"))
        check_pending();

    if (!strcmp(argv[2], "fork")) {
        int pid = fork();
        if (pid == 0) {
            printf("Child: \n");
            if (!strcmp(argv[1], "pending")) { // not pending
                check_pending();
            } else {
                raise(SIGUSR1);
            }
            printf("Fork: program have benn executed correctly\n");
            exit(0);
        }
    } else if(strcmp(argv[1], "exec") == 0) {
        execl("./exec", "./exec", argv[1], NULL);
    }
    return 0;
}
