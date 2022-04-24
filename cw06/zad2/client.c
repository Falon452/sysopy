//
// Created by Damian Tworek on 21/04/2022.
//

#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

#include "common.h"

mqd_t main_que;
mqd_t my_que;
int my_id;
char path[250];

void receive_message() {
    msgbuf from_other;
    uint mtype;

    if (mq_receive(my_que, (char *) &from_other, size, &mtype) == -1) {
        printf("error receiving msg\n");
        exit(1);
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    printf("> New message from ID: %d\n%s\n\n%d-%02d-%02d %02d:%02d:%02d\n", from_other.sender_pid, from_other.mtext,
           tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void send_stop();

void stop() {
    send_stop();
    mq_close(main_que);
    mq_unlink(path);
    exit(1);
}

void send_stop() {
    msgbuf to_server;
    to_server.sender_pid = my_id;
    to_server.mtext[0] = '\0';
    mq_send(main_que, (char *) &to_server, size, STOP);
}

void to_one(int to_id, char* text) {
    msgbuf to_server;
    to_server.sender_pid = my_id;
    to_server.receiver_id = to_id;
    strncpy(to_server.mtext, text, MAX_MSG_LEN);

    if (mq_send(main_que, (char *) &to_server, size, TO_ONE) == -1) {
        fprintf(stderr, "send_message can't send mssg to server\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }
}

void to_all(char* text){
    msgbuf to_server;
    to_server.sender_pid = my_id;
    strncpy(to_server.mtext, text, MAX_MSG_LEN);

    if (mq_send(main_que, (char *) &to_server, size, TO_ALL) == -1) {
        fprintf(stderr, "msgsnd can't send msg to server\n");
        exit(1);
    }
}



void send_init(const char * path){
    msgbuf to_server;
    to_server.sender_pid = getpid();
    sprintf(to_server.mtext, "%s", path);


    if (mq_send(main_que, (char *) &to_server, size, INIT) == -1) {
        fprintf(stderr, "send_init can't send mssg to server\n");
        exit(1);
    }

    msgbuf from_server;


    if (mq_receive(my_que, &from_server, size, NULL) == -1) {
        fprintf(stderr, "send_init can't receive msg from server\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    my_id = atoi(from_server.mtext);
    printf("Your ID is %d\n", my_id);
    fflush(stdout);
}


void send_list() {
    msgbuf to_server;
    to_server.sender_pid = my_id;
    to_server.mtext[0] = '\0';

    if (mq_send(main_que, (char *) &to_server, size, LIST) == -1) {
        fprintf(stderr, "msgsnd can't send msg to server\n");
        exit(1);
    }
}


int main() {
    if (atexit(stop) != 0) {
        printf("atexit error\n");
        exit(1);

    }

    if (signal(SIGINT, stop) == SIG_ERR || signal(SIGQUIT, stop) == SIG_ERR) {
        printf("atexit error");
        exit(1);
    }
    if (signal(SIGUSR1, receive_message) == SIG_ERR) {
        printf("error setting action for sigusr1\n");
        exit(1);
    }


    sprintf((char *) path, "/%d", getpid());

    main_que = open_que(SERVER);

    my_que = create_queue(path);

    send_init(path);

    struct sigevent notify_handler;
    notify_handler.sigev_notify = SIGEV_SIGNAL;
    notify_handler.sigev_signo = SIGUSR1;

    mq_notify(my_que, &notify_handler);

    char input[MAX_MSG_LEN];
    const char delimiter[1] = " ";
    char * token;
    char text[MAX_MSG_LEN];

    while (1) {
        text[0] = '\0';
        gets(input);
        token = strtok(input, delimiter);

        if (strcmp(token, "LIST") == 0) {
            send_list();
        }
        else if (strcmp(token, "STOP") == 0) {
            stop();
        }
        else if (strcmp(token, "2ONE") == 0) {

            int receiver_id = -1;

            token = strtok(NULL, " ");
            receiver_id = atoi(token);

            token = strtok(NULL, "\n");
            strncat(text, token, MAX_MSG_LEN);

            to_one(receiver_id, text);
        }
        else if(strcmp(token, "2ALL") == 0) {

            token = strtok(NULL, "\n");
            strncat(text, token, MAX_MSG_LEN);
            to_all(text);
        }

    }
}