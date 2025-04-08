#ifndef STRING_H
#define STRING_H

#include "stddef.h"

int strcmp(const char* str1, const char* str2);
char* strcpy(char* dest, const char* src);
size_t strlen(const char* str);
char* strcat(char* dest, const char* src);
char* strtok_r(char* str, const char* delim, char** saveptr);
size_t strspn(const char* str, const char* accept);
char* strpbrk(const char* str, const char* accept);
char* strchr(const char* str, int c);

#endif // STRING_H