
#include "test.h"
#include "../libs/stdio.h"

void test() {
    // Test the printf function
    printf("Hello, World!\n");
    printf("Integer: %d\n", 42);
    printf("Hexadecimal: %x\n", 0xDEADBEEF);
    printf("String: %s\n", "This is a test string.");
    printf("Character: %c\n", 'A');

    // Test the println function
    println("This is a test for println.");
}