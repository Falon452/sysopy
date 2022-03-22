//
// Created by debian on 3/19/22.
//
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>


#define DIR_MAX_LEN 250
#define CWD_MAX_LEN 250
#define PATH_MAX_LEN 300

int no_regular_files = 0;
int no_block_device_files = 0;
int no_character_device_files = 0;
int no_directory_files = 0;
int no_FIFO_files = 0;
int no_slink_files = 0;
int no_socket_files = 0;
int no_unknown_files = 0;


void file_stats(const char* filepath) {
    struct stat sb;
    char absolute_path[PATH_MAX_LEN];

    if (lstat(filepath, &sb) == -1) {
        perror("stat\n");
        return;
    }


    realpath(filepath, absolute_path);


    printf("Filepath:                 %s\n", absolute_path);

    printf("File type:                ");

    switch (sb.st_mode & S_IFMT) {
        case S_IFBLK:  printf("block device\n");     no_block_device_files++;       break;
        case S_IFCHR:  printf("character device\n"); no_character_device_files++;   break;
        case S_IFDIR:  printf("directory\n");        no_directory_files++;          break;
        case S_IFIFO:  printf("FIFO/pipe\n");        no_FIFO_files++;          break;
        case S_IFLNK:  printf("symlink\n");          no_slink_files++;              break;
        case S_IFREG:  printf("regular file\n");     no_regular_files++;            break;
        case S_IFSOCK: printf("socket\n");           no_socket_files++;             break;
        default:       printf("unknown?\n");         no_unknown_files++;            break;
    }

    printf("Link count:               %ju\n", (uintmax_t) sb.st_nlink);

    printf("File size:                %jd bytes\n",
           (intmax_t) sb.st_size);

    printf("Last file access:         %s", ctime(&sb.st_atime));
    printf("Last file modification:   %s\n", ctime(&sb.st_mtime));
    //    printf("Ownership:                UID=%ju   GID=%ju\n",
//           (uintmax_t) sb.st_uid, (uintmax_t) sb.st_gid);
    //    printf("I-node number:            %ju\n", (uintmax_t) sb.st_ino);
//
//    printf("Mode:                     %jo (octal)\n",
//           (uintmax_t) sb.st_mode);

}


void search_dir(char* dirpath) {
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
            if ((dp -> d_type) == DT_DIR)
                search_dir(relative_path);

            file_stats(relative_path);
        }
        relative_path[0] = '\0';
    }
    closedir(dir);
}




int main(int argc, char* argv[]) {
    if (argc > 2) {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argc == 1) {
        char dir_name[DIR_MAX_LEN];
        printf("Enter directory: ");
        scanf("%s", dir_name);

        printf("\nEntered directory: %s\n", dir_name); //! d

        search_dir(dir_name);
    } else
        search_dir(argv[1]);

    printf("----------------------------------------\n");
    printf("Regular files          count:   %d\n", no_regular_files);
    printf("Directory files        count:   %d\n", no_directory_files);
    printf("Block device files     count:   %d\n", no_block_device_files);
    printf("Character device files count:   %d\n", no_character_device_files);
    printf("FIFO/pipe files        count:   %d\n", no_FIFO_files);
    printf("Symbolic link files    count:   %d\n", no_slink_files);
    printf("Socket files           count:   %d\n", no_socket_files);
    printf("Unknown files          count:   %d\n", no_unknown_files);

}