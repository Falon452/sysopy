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
#include <mqueue.h>
#include <sys/stat.h>
#include <mqueue.h>


mqd_t main_que;
int users_pid[MAX_USERS];
mqd_t users_que_pid[MAX_USERS];
char file_log[MAX_MSG_LEN];
FILE *log_fd;


void log_msg(msgbuf *msg, uint msg_type) {
    char buf[300];
    strncpy(buf, "", MAX_MSG_LEN);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    switch(msg_type) {
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

    printf("\n%d-%02d-%02d %02d:%02d:%02d\n\n",
           tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(log_fd, "\n%d-%02d-%02d %02d:%02d:%02d\n\n\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void msg_to_all(msgbuf *msg) {
    for (int i = 0; i < MAX_USERS; i ++) {
        if (users_pid[i] != 0 && i != msg->sender_pid) {
            if (mq_send(users_que_pid[i], msg, size, TO_ALL) == -1 ) {
                fprintf(stderr, "Can't send message to all\n");
                exit(1);
            }
            kill(users_pid[i], SIGUSR1);
        }
    }
}

void msg_to_one(msgbuf *msg) {
    if (mq_send(users_que_pid[msg->receiver_id], (char *) msg, size, TO_ONE) == -1 ) {
        fprintf(stderr, "Can't send message to someone\n");
        exit(1);
    }
    kill(users_pid[msg->receiver_id], SIGUSR1);
}


void mclose(){
    fprintf(log_fd, "closing\n");
    fclose(log_fd);

    for (int i = 0; i < MAX_USERS; i++) {
        if (users_pid[i] != 0) {
            mq_close(users_que_pid[i]);
            kill(users_pid[i], SIGINT);
        }
    }
    mq_unlink(SERVER);
    exit(1);
}

void list(msgbuf *msg) {
    msgbuf to_client;
    to_client.sender_pid = getpid();

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

    if (mq_send(users_que_pid[msg->sender_pid], (char *) &to_client, size, LIST) == -1) {
        fprintf(stderr, "i just can not send this message... sorry\n");
        exit(1);
    }

    kill(users_pid[msg->sender_pid], SIGUSR1);
}


void init(msgbuf *msg) {
    int i = 0;
    for (; i < MAX_USERS; i++) {
        if (users_pid[i] == 0) {
            users_pid[i] = msg->sender_pid;
            users_que_pid[i] = open_que(msg->mtext);

            break;
        }
    }

    msgbuf to_client;
    to_client.sender_pid = getpid();
    sprintf(to_client.mtext, "%d", i);

    if(mq_send(users_que_pid[i], (char*) &to_client, size, INIT) == -1){
        fprintf(stderr, "can't send init message \n");
        exit(1);
    }

}


void disconnect(msgbuf *msg) {
    int i = msg->sender_pid;
    users_pid[i] = 0; // in this case I sent ID
    users_que_pid[i] = 0;
    printf("disconnected at %i\n", i);
}


void handle_msg(msgbuf *msg, uint msg_type) {
    switch (msg_type) {
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
    log_msg(msg, msg_type);
}


int main() {
    if (signal(SIGINT, mclose) == SIG_ERR) {
        printf("error setting action for SIGINT\n");
        exit(1);
    }
    if (signal(SIGQUIT, mclose) == SIG_ERR) {
        printf("error setting action for SIGQUIT\n");
        exit(1);
    }

    for (int i = 0; i < MAX_USERS; i++ ) {
        users_pid[i] = 0;
        users_que_pid[i] = 0;
    }


    strncpy(file_log, "server_logs.txt", MAX_MSG_LEN);

    printf("---server---\n");
    char* path = getenv("HOME");

    main_que = create_queue(SERVER);
    printf("main_que=%d\n", main_que);

    if ((log_fd = fopen(file_log, "w")) == NULL ) {
        fprintf(stderr, "i cant open a file log... sorry\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }


    msgbuf msg;
    uint msg_type;

    while(1){
        if(mq_receive(main_que, &msg, size, &msg_type) == -1){
            fprintf(stderr, "server: error receiving message\n");
            exit(1);
        }
        handle_msg(&msg, msg_type);
    }

    fclose(log_fd);
}