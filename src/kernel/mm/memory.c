#include "memory.h"
#include "../terminal.h"
#include "../../libs/stdio.h"

// Define the size of the heap
#define HEAP_SIZE (1024 * 1024) // 1 MB

// Memory block structure
typedef struct memory_block {
    size_t size;
    int free;
    struct memory_block* next;
} memory_block_t;

// Define the heap and free list
static uint8_t heap[HEAP_SIZE];
static memory_block_t* free_list = NULL;

// Align size to 8 bytes
static size_t align(const size_t size) {
    return (size + 7) & ~7;
}

// Initialize the free list
static void initialize_memory() {
    free_list = (memory_block_t*)heap;
    free_list->size = HEAP_SIZE - sizeof(memory_block_t);
    free_list->free = 1;
    free_list->next = NULL;
}

// Allocate memory
void* malloc(size_t size) {
    if (!free_list) {
        initialize_memory();
    }

    size = align(size);
    memory_block_t* current = free_list;

    while (current) {
        if (current->free && current->size >= size) {
            // Split block if there's excess space
            if (current->size > size + sizeof(memory_block_t)) {
                memory_block_t* new_block = (memory_block_t*)((uint8_t*)current + sizeof(memory_block_t) + size);
                new_block->size = current->size - size - sizeof(memory_block_t);
                new_block->free = 1;
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;
            }

            current->free = 0;
            return (uint8_t*)current + sizeof(memory_block_t);

        }
        current = current->next;
    }

    terminal_write("Out of memory!\n");
    return NULL;
}

// Free memory
void free(void* ptr) {
    if (!ptr) return;

    memory_block_t* block = (memory_block_t*)((uint8_t*)ptr - sizeof(memory_block_t));
    block->free = 1;

    // Merge adjacent free blocks
    memory_block_t* current = free_list;
    while (current) {
        if (current->free && current->next && current->next->free) {
            current->size += current->next->size + sizeof(memory_block_t);
            current->next = current->next->next;
        }
        current = current->next;
    }
}

// Reallocate memory
void* realloc(void* ptr, const size_t size) {
    if (!ptr) {
        return malloc(size);
    }

    const memory_block_t* block = (memory_block_t*)((uint8_t*)ptr - sizeof(memory_block_t));
    if (block->size >= size) {
        return ptr; // Existing block is large enough
    }

    void* new_ptr = malloc(size);
    if (new_ptr) {
        const size_t copy_size = block->size < size ? block->size : size;
        for (size_t i = 0; i < copy_size; i++) {
            ((uint8_t*)new_ptr)[i] = ((uint8_t*)ptr)[i];
        }
        free(ptr);
    }
    return new_ptr;
}