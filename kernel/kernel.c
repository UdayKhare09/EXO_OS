// kernel/kernel.c
// EXO_OS kernel

// Include headers
#include "terminal.h"
#include "keyboard.h"
#include "shell.h"

// Define any additional types needed by kernel
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

// Kernel entry point
void kernel_main() {
    // Initialize subsystems
    terminal_initialize();
    keyboard_initialize();

    // Display welcome message
    terminal_write("Welcome to EXO_OS!\n");
    terminal_write("32-bit protected mode kernel initialized.\n");

    // Initialize and run the shell
    shell_initialize();
    shell_run();

    // We should never get here
}

void __attribute__((section(".entry"))) _start(void) {
    // Clear interrupts
    __asm__ volatile("cli");

    // Call the kernel main function
    kernel_main();

    // Halt the CPU if kernel returns
    __asm__ volatile("hlt");

    // Infinite loop as a fallback
    for(;;);
}