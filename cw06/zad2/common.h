//
// Created by Damian Tworek on 21/04/2022.
//

#ifndef CW05_COMMON_H
#define CW05_COMMON_H

#include "signal.h"
#include <time.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#define MAX_MSG_LEN 250
#define MAX_USERS 30

#define STOP 1
#define LIST 2
#define TO_ALL 3
#define TO_ONE 4
#define INIT 5

#define SERVER "/server_que"
#define PROJECT_ID 'A'



typedef struct {
    pid_t sender_pid;
    char mtext[MAX_MSG_LEN];
    int receiver_id;
} msgbuf;


int size = sizeof(msgbuf);


mqd_t create_queue(const char* path){
    struct mq_attr attr;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = size;

    mqd_t q_id = mq_open(path, O_RDONLY | O_CREAT | O_EXCL, 0660, &attr);

    if(q_id == -1){
        fprintf(stderr, "error creating que\n");
        exit(1);
    }
    return q_id;
}

mqd_t open_que(const char* path){
    mqd_t q_id = mq_open(path, O_WRONLY);
    if(q_id == -1){
        fprintf(stderr, "error opening que\n");
        exit(1);
    }
    return q_id;
}
#endif //CW05_COMMON_H