#include "memory.h"
#include "../terminal.h"

// Define the size of the heap
#define HEAP_SIZE 1024 * 1024 // 1 MB

// Define the heap structure
static uint8_t heap[HEAP_SIZE];
static size_t heap_index = 0;

// Allocate memory
void* malloc(size_t size) {
    if (heap_index + size > HEAP_SIZE) {
        terminal_write("Out of memory!\n");
        return NULL;
    }

    void* ptr = &heap[heap_index];
    heap_index += size;
    return ptr;
}

// Free memory (simple implementation, does not actually free memory)
void free(void* ptr) {
    // In this simple implementation, free does nothing
    (void)ptr;
}

// Reallocate memory
void* realloc(void* ptr, size_t size) {
    void* new_ptr = malloc(size);
    if (new_ptr && ptr) {
        // Copy old data to new location
        size_t old_size = (uint8_t*)ptr - heap;
        if (old_size > size) {
            old_size = size;
        }
        for (size_t i = 0; i < old_size; i++) {
            ((uint8_t*)new_ptr)[i] = ((uint8_t*)ptr)[i];
        }
    }
    return new_ptr;
}