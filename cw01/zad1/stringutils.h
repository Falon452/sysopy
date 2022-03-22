//
// Created by Damian Tworek on 13/03/2022.
//
#pragma once
#ifndef SYSOPY1_STRINGUTILS_H
#define SYSOPY1_STRINGUTILS_H

#endif //SYSOPY1_STRINGUTILS_H


int str_len(char* str);  // does not count '\0'

int number_of_chars_in_array(char** array, int n);

char* flatten_array_of_strings(char **source, char*result, int number_of_files);  // after usage, call free(result)