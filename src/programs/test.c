#include "test.h"
#include "../libs/stdio.h"
#include "shell.h"

// Correct the function signature to return int as per cmd_function type
static int cmd_test_func(int argc, char** argv) {
    // Test anything you want here



    return 0;  // Return success
}

void test() {
    // Register the test command with the shell
    const int result = shell_register_command("c_test", cmd_test_func, "Custom test command");

    if (result == 0) {
        printf("Custom test command registered successfully.\n");
        printf("Try it by typing 'c_test arg1 arg2' in the shell.\n");
    } else {
        printf("Failed to register custom test command (command registry full).\n");
    }
}