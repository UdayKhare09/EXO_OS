// src/libs/string.c
#include "string.h"

int strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

char* strcpy(char* dest, const char* src) {
    char* original_dest = dest;
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    return original_dest;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

char* strcat(char* dest, const char* src) {
    char* original_dest = dest;

    // Find the end of the destination string
    while (*dest) {
        dest++;
    }

    // Append the source string
    while (*src) {
        *dest++ = *src++;
    }

    // Add null terminator
    *dest = '\0';

    return original_dest;
}

// Find first occurrence of character c in str
char* strchr(const char* str, int c) {
    while (*str != (char)c) {
        if (*str == '\0') {
            return NULL;
        }
        str++;
    }
    return (char*)str;
}

// Count leading characters in str that are in accept
size_t strspn(const char* str, const char* accept) {
    size_t count = 0;
    while (*str && strchr(accept, *str++)) {
        count++;
    }
    return count;
}

// Find first occurrence of any character in accept within str
char* strpbrk(const char* str, const char* accept) {
    while (*str) {
        if (strchr(accept, *str)) {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}