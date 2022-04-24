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

#include "common.h"

int main_que;
int my_que;
int my_id;


void receive_message() {
    msgbuf from_other;
    if (msgrcv(my_que, &from_other, size, -INIT - 1, 0) == -1) {
        printf("error receiving msg\n");
        exit(1);
    }
    struct tm tm = from_other.tm;

    printf("> New message from ID: %d\n%s\n\n%d-%02d-%02d %02d:%02d:%02d\n", from_other.sender_pid, from_other.mtext,
           tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

}

void send_stop();

void stop() {
    msgctl(my_que, IPC_RMID, NULL);
    send_stop();
    exit(1);
}

void send_stop() {
    msgbuf to_server;
    to_server.mtype = STOP;
    to_server.sender_pid = my_id;

    if (msgsnd(main_que, &to_server, size, 0) == -1) {
        fprintf(stderr, "send_message can't send mssg to server\n");
        exit(1);
    }
}

void send_message(int to_id, char* text) {
    msgbuf to_server;
    to_server.mtype = TO_ONE;
    to_server.sender_pid = my_id;
    to_server.receiver_id = to_id;
    strncpy(to_server.mtext, text, MAX_MSG_LEN);

    if (msgsnd(main_que, &to_server, size, 0) == -1) {
        fprintf(stderr, "send_message can't send mssg to server\n");
        exit(1);
    }
}

void to_all(char* text){
    msgbuf to_server;
    to_server.mtype = TO_ALL;
    to_server.sender_pid = my_id;
    strncpy(to_server.mtext, text, MAX_MSG_LEN);

    if (msgsnd(main_que, &to_server, size, 0) == -1) {
        fprintf(stderr, "msgsnd can't send msg to server\n");
        exit(1);
    }
}


int create_queue(const char *path, int id) {
    key_t unique_key = ftok(path, id);
    if (unique_key == -1) {
        fprintf(stderr, "ftok error: path does not exist or if it cannot be accessed by the calling");
        exit(1);
    }

    int que_id = msgget(unique_key, IPC_CREAT | IPC_EXCL | 0666);

    if (que_id == -1) {
        fprintf(stderr, "Error in creating queue.");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    return que_id;
}


void send_init(){
    msgbuf to_server;
    to_server.mtype = INIT;
    to_server.sender_pid = getpid();
    sprintf(to_server.mtext, "%d", my_que);

    if (msgsnd(main_que, &to_server, size, 0) == -1) {
        fprintf(stderr, "send_init can't send mssg to server\n");
        exit(1);
    }

    msgbuf from_server;

    printf("waiting for receive\n");

    if (msgrcv(my_que, &from_server, size, INIT, 0) == -1) {
        fprintf(stderr, "send_init can't receive msg from server\n");
        exit(1);
    }

    my_id = atoi(from_server.mtext);
    printf("Your ID is %d\n", my_id);
    fflush(stdout);
}


void send_list() {
    msgbuf to_server;
    to_server.mtype = LIST;
    to_server.sender_pid = my_id;
    to_server.mtext[0] = '\0';

    if (msgsnd(main_que, &to_server, size, 0) == -1) {
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

    char* path = getenv("HOME");
    key_t unique_key = ftok(path, PROJECT_ID);

    main_que = msgget(unique_key, 0);

    my_que = create_queue(path, (getpid() % ('Z' - 'a' + 1)) + 'a');

    send_init();

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

            send_message(receiver_id, text);
        }
        else if(strcmp(token, "2ALL") == 0) {

            token = strtok(NULL, "\n");
            strncat(text, token, MAX_MSG_LEN);
            to_all(text);
        }

    }
}