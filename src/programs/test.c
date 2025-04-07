
#include "test.h"
#include "../libs/stdio.h"
#include "../kernel/mm/memory.h"

void test() {
    // Test the malloc function
    char* buffer = (char*)malloc(100);
    if (buffer) {
        printf("Allocated 100 bytes of memory.\n");
        // Use the buffer
        for (int i = 0; i < 100; i++) {
            buffer[i] = 'A' + (i % 26);
        }
        buffer[99] = '\0'; // Null-terminate the string
        printf("Buffer content: %s\n", buffer);
    } else {
        printf("Memory allocation failed.\n");
    }
    // Test the free function
    free(buffer);
    printf("Freed allocated memory.\n");
    // Test the realloc function
    char* new_buffer = (char*)malloc(50);
    if (new_buffer) {
        printf("Allocated 50 bytes of memory.\n");
        // Use the new buffer
        for (int i = 0; i < 50; i++) {
            new_buffer[i] = 'B' + (i % 26);
        }
        new_buffer[49] = '\0'; // Null-terminate the string
        printf("New buffer content: %s\n", new_buffer);
    } else {
        printf("Memory allocation failed.\n");
    }
    free(new_buffer);
    printf("Freed new buffer memory.\n");

    printf("Test completed.\n");
}