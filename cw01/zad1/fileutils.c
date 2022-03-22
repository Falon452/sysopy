//
// Created by Damian Tworek on 13/03/2022.
//

#include <stdio.h>

int count_lines(char* filename)
{
    int count = 0;
    FILE* fp;
    char c;

    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Could not open file %s", filename);
        return 0;
    }

    for (c = getc(fp); c != EOF; c = getc(fp)) {
        printf("%c", c);
        if (c == '\n')
            count = count + 1;
    }

    fclose(fp);

    return count;
}