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
#include <fcntl.h>


int main_que;
int users_pid[MAX_USERS];
int users_que_pid[MAX_USERS];
char file_log[MAX_MSG_LEN];
FILE *log_fd;

void log_msg(msgbuf *msg) {
    char buf[300];
    strncpy(buf, "", MAX_MSG_LEN);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    switch(msg->mtype) {
        case STOP:
            printf("STOP %d\n", msg->sender_pid);
            fprintf(log_fd, "STOP %d\n", msg->sender_pid);
            break;
        case LIST:
            printf("LIST requested by %d\n", msg->sender_pid);
            fprintf(log_fd, "LIST requested by %d\n", msg->sender_pid);
            break;
        case TO_ALL:
            printf("TO_ALL from %d\n", msg->sender_pid);
            fprintf(log_fd, "TO_ALL from %d\n", msg->sender_pid);
            printf("%s\n", msg->mtext);
            fprintf(log_fd, "%s\n" , msg->mtext);
            break;
        case TO_ONE:
            printf("TO_ONE from: %d to: %d\n", msg->sender_pid, msg->receiver_id);
            fprintf(log_fd, "TO_ONE from: %d to: %d\n", msg->sender_pid, msg->receiver_id);
            printf("%s\n", msg->mtext);
            fprintf(log_fd, "%s\n" , msg->mtext);
            break;
        case INIT:
            printf("INIT PID = %d\n", msg->sender_pid);
            fprintf(log_fd, "INIT PID= %d\n", msg->sender_pid);
            break;
    }

    printf("\n%d-%02d-%02d %02d:%02d:%02d\n",
           tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(log_fd, "\n%d-%02d-%02d %02d:%02d:%02d\n\n\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void msg_to_all(msgbuf *msg) {
    time_t t = time(NULL);
    msg->tm = *localtime(&t);

    for (int i = 0; i < MAX_USERS; i ++) {
        if (users_pid[i] != 0 && i != msg->sender_pid) {
            if (msgsnd(users_que_pid[i], msg, size, 0) == -1 ) {
                fprintf(stderr, "Can't send message to someone\n");
                exit(1);
            }
            kill(users_pid[i], SIGUSR1);
        }
    }
}

void msg_to_one(msgbuf *msg) {
    msgbuf msg_to_some1;
    msg_to_some1.mtype = TO_ONE;
    msg_to_some1.sender_pid = msg->sender_pid; // this is not pid but ID this time
    msg_to_some1.receiver_id = msg->receiver_id;

    time_t t = time(NULL);
    msg_to_some1.tm = *localtime(&t);

    sprintf(msg_to_some1.mtext, "%s", msg->mtext);

    if (msgsnd(users_que_pid[msg->receiver_id], &msg_to_some1, size, 0) == -1 ) {
        fprintf(stderr, "Can't send message to someone\n");
        exit(1);
    }
    kill(users_pid[msg->receiver_id], SIGUSR1);
}


void close_queue(){
    fprintf(log_fd, "closing\n");
    fclose(log_fd);
    for (int i = 0; i < MAX_USERS; i++) {
        kill(users_pid[i], SIGINT);
    }
    msgctl(main_que, IPC_RMID, NULL);
    exit(1);
}

void list(msgbuf *msg) {
    msgbuf to_client;
    to_client.sender_pid = getpid();
    to_client.mtype = LIST;

    time_t t = time(NULL);
    to_client.tm = *localtime(&t);

    strncpy(to_client.mtext, "", MAX_MSG_LEN);

    strncat(to_client.mtext, "-----users------\n", MAX_MSG_LEN - strlen(to_client.mtext));

    strncat(to_client.mtext, "id  --  pid\n", MAX_MSG_LEN - strlen(to_client.mtext));
    for (int i = 0; i < MAX_USERS; i++) {
        if (users_pid[i] != 0) {
            char tmp[20];
            sprintf(tmp, "%d   -- %d\n", i, users_pid[i]);
            strncat(to_client.mtext, tmp, MAX_MSG_LEN - strlen(to_client.mtext));
        }
    }

    if (msgsnd(users_que_pid[msg->sender_pid], &to_client, size, 0) == -1) {
        fprintf(stderr,"i just can not send this message... sorry\n");
        exit(1);
    }

    kill(users_pid[msg->sender_pid], SIGUSR1);
}


void init(msgbuf *msg) {
    int i = 0;
    for (; i < MAX_USERS; i++) {
        if (users_pid[i] == 0) {
            users_pid[i] = msg->sender_pid;
            users_que_pid[i] = atoi(msg->mtext);
            break;
        }
    }

    msgbuf to_client;
    to_client.sender_pid = getpid();
    to_client.mtype = INIT;
    sprintf(to_client.mtext, "%d", i);

    printf("New connect at %d\n", i);

    msgsnd(users_que_pid[i], &to_client, size, 0);
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

void disconnect(msgbuf *msg) {
    int i = msg->sender_pid;
    users_pid[i] = 0; // in this case I sent ID
    users_que_pid[i] = 0;
    printf("disconnected at %i\n", i);
}


void handle_msg(msgbuf *msg) {
    switch (msg->mtype) {
        case LIST:
            list(msg);
            break;
        case INIT:
            init(msg);
            break;
        case TO_ONE:
            msg_to_one(msg);
            break;
        case TO_ALL:
            msg_to_all(msg);
            break;
        case STOP:
            disconnect(msg);
            break;
    }
    log_msg(msg);
}


int main() {
    if(atexit(close_queue) != 0){
        printf("atexit error\n");
        exit(1);
    }

    if (signal(SIGINT, close_queue) == SIG_ERR) {
        printf("error setting action for SIGINT\n");
        exit(1);
    }
    if (signal(SIGQUIT, close_queue) == SIG_ERR) {
        printf("error setting action for SIGQUIT\n");
        exit(1);
    }

    for (int i = 0; i < MAX_USERS; i++ ) {
        users_pid[i] = 0;
        users_que_pid[i] = 0;
    }


    strncpy(file_log, "server_logs.txt", MAX_MSG_LEN);

    if ((log_fd = fopen(file_log, "w")) == NULL ) {
        fprintf(stderr, "i cant open a file log... sorry\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }

    printf("---server---\n");
    char* path = getenv("HOME");


    main_que = create_queue(path, PROJECT_ID);

    msgbuf msg;
    while(1){

        if(msgrcv(main_que, &msg, size, 0, 0) == -1){
            fprintf(stderr, "server: error receiving message\n");
            exit(1);
        }

        handle_msg(&msg);
    }

    fclose(log_fd);
}