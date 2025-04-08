// EXO_OS kernel

// Include headers
#include "terminal.h"
#include "drivers/keyboard.h"
#include "../programs/test.h"

// Define any additional types needed by kernel
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

// Kernel entry point
void kernel_main() {
	keyboard_initialize();
    terminal_initialize();
	test();
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