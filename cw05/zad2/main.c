//
// Created by Damian Tworek on 15/04/2022.
//
#include <string.h>
#include <stdio.h>
#define MAX_BUFFER 1000

void print_mail(char *type) {
    if (strcmp(type, "data")) {
        FILE *popen_file = popen("mail","w");
        pclose(popen_file);
    } else if (strcmp(type, "nadawca")) {
        FILE *popen_file = popen("mail | sort -k 9","w");
        pclose(popen_file);
    }
}

void send_mail(char *address, char *title, char *content) {
    FILE *fp;
    char buffer[MAX_BUFFER];
    sprintf(buffer, "mail \"%s\" \"%s\"", title, address);
    fp = popen(buffer, "w");
    fputs(content, fp);
    pclose(fp);
}

int main(int argc, char *argv[]){
    if(!(argc == 2 || argc == 4)){
        printf("(1) Usage: %s <adresEmail> <tytuł> <treść>\n", argv[0]);
        printf("(1) Usage: %s nadawca|data\n",argv[0]);
        return 2;
    } else if(argc == 2){
        if ((strcmp(argv[1], "nadawca") == 0)  || (strcmp(argv[1], "data") == 0) ) {
            print_mail(argv[1]);
        } else {
            printf("(1) Usage: %s nadawca|data\n",argv[0]);
            return 2;
        }
    } else {
        send_mail(argv[1], argv[2], argv[3]);
    }
    return 0;
}