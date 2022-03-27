//
// Created by debian on 3/26/22.
//
#define PATH_MAX_LEN 250
#define MAX_LINE 500

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

void search_dir(char* dirpath, char* pattern, int max_depth, int depth) {
    if (depth > max_depth) {
        return;
    }
    DIR *dir;
    struct dirent *dp;
    char relative_path[PATH_MAX_LEN];

    if ((dir = opendir(dirpath)) == NULL) {
        char error_message[250];
        sprintf(error_message, "Cannot open %s\n", dirpath);
        perror(error_message);
        return;
    }

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp -> d_name, ".") != 0 && strcmp(dp -> d_name, "..") != 0) {
            strcpy(relative_path, dirpath);
            strcat(relative_path, "/");
            strcat(relative_path, dp -> d_name);

            if ((dp -> d_type) == DT_DIR) {
                pid_t child_pid;
                if ((child_pid = vfork()) == 0 ) {
                    search_dir(relative_path, pattern, max_depth, depth++);
                    exit(0);
                }
            }
            if (strstr((dp -> d_name), ".txt") == (dp -> d_name) + strlen(dp -> d_name) - 4){
                FILE *fp;
                fp = fopen(relative_path, "r");

                if (fp == NULL){
                    continue;
                }

                char *buffor = calloc(MAX_LINE, sizeof(char));
                while (fgets(buffor, MAX_LINE, fp) != NULL){  //reads line from fp to buffor
                    char * pch;
                    pch = strstr(buffor, pattern);  // checks if pattern is in buffor
                    if (pch != NULL){
                        printf("%s PID: %d\n", relative_path, (int)getpid());
                        break;
                    }
                }

                fclose(fp);
                free(buffor);
            }
        }
        relative_path[0] = '\0';
    }

    closedir(dir);
    exit(0);
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s directory-path pattern-to-find max-recursion\n", argv[0]);
        exit(1);
    }
    char* dir_path = argv[1];
    char* pattern = argv[2];
    int max_recursion = atoi(argv[3]);

    if (max_recursion < 0) {
        printf("Max recursion must be >= 0.\n");
        exit(1);
    }

    pid_t child_pid;

    if ((child_pid = vfork()) == 0 ) {
        search_dir(dir_path, pattern, max_recursion, 0);
        exit(0);
    }


}