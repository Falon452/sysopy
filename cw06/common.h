//
// Created by Damian Tworek on 21/04/2022.
//

#ifndef CW05_COMMON_H
#define CW05_COMMON_H

#include "signal.h"
#include <time.h>

#define MAX_MSG_LEN 250
#define MAX_USERS 30

#define STOP 1
#define LIST 2
#define TO_ALL 3
#define TO_ONE 4
#define INIT 5

#define PROJECT_ID 'A'

#endif //CW05_COMMON_H

typedef struct {
    long mtype;
    char mtext[MAX_MSG_LEN];
    pid_t sender_pid;
    int receiver_id;
    struct tm tm;
} msgbuf;

int size = sizeof(msgbuf) - sizeof(long);