//
// Created by debian on 4/5/22.
//
#include <signal.h>
#include <stdio.h>
void handler() {
    printf("I will be reseted to deafult\n");
}

int main(int argc, char* argv[]) {
    struct sigaction action;
    action.sa_flags = SA_RESETHAND;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action, NULL);

    raise(SIGINT);
    raise(SIGINT);
    while(1);
}
