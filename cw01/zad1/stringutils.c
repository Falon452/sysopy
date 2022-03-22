//
// Created by Damian Tworek on 13/03/2022.
//


#include <string.h>
#include <stdio.h>


int str_len(char* str) {
    if (str == NULL)
        return 0;
    int i = 0, len = 0;
    while (str[i] != '\0') {
        len += 1;
        i++;
    }
    return len;
}


int number_of_chars_in_array(char** array, int n) {
    if (array == NULL) {
        return 0;
    }
    int result = 0;
    int j = 0;
    for (int i = 0; i < n; i++) {
        j = 0;
        result += str_len(array[i]);
    }
    return result;
}


char* flatten_array_of_strings(char **source, char*result, int number_of_files){
    if (source == NULL) {
        printf("WARNING: Source is NULL.\n");
        return NULL;
    }
    if (number_of_files < 0) {
        printf("WARNING: Number of files must >= 0.\n");
        return NULL;
    }
    if (result == NULL) {
        printf("WARNING: Result is NULL.\n");
        return NULL;
    }

    char* ptr;
    ptr = result;

    int result_index = 0;
    for (int i = 0; i < number_of_files; i++) {
        int j = 0;
        while (source[i][j] != '\0') {
            ptr[result_index] = source[i][j];
            result_index++;
            j++;
        }
        ptr[result_index] = ' ';
        result_index++;
    }

    result[result_index - 1] = '\0';  // the last one was ' '
    return result;
}