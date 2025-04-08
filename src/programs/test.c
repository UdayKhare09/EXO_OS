#include "test.h"
#include "../libs/stdio.h"
#include "../kernel/mm/memory.h" // For malloc and free

void test() {
    // Dynamically allocate memory for the cells
    char* cells = (char*)malloc(9 * sizeof(char));
    if (!cells) {
        println("Memory allocation failed!");
        return;
    }

    // Initialize the cells
    for (int i = 0; i < 9; i++) {
        cells[i] = '1' + i;
    }

    int player = 1;
    int choice;
    int moves = 0;

    while (1) {
        // Print the board
        println("Tic Tac Toe");
        printf(" %c | %c | %c \n", cells[0], cells[1], cells[2]);
        println("---|---|---");
        printf(" %c | %c | %c \n", cells[3], cells[4], cells[5]);
        println("---|---|---");
        printf(" %c | %c | %c \n", cells[6], cells[7], cells[8]);

        // Check for a winner or draw
        if ((cells[0] == cells[1] && cells[1] == cells[2]) || (cells[3] == cells[4] && cells[4] == cells[5]) || (cells[6] == cells[7] && cells[7] == cells[8]) || // Rows
            (cells[0] == cells[3] && cells[3] == cells[6]) || (cells[1] == cells[4] && cells[4] == cells[7]) || (cells[2] == cells[5] && cells[5] == cells[8]) || // Columns
            (cells[0] == cells[4] && cells[4] == cells[8]) || (cells[2] == cells[4] && cells[4] == cells[6])) {                                               // Diagonals
            println("Player %d wins!", (player == 1) ? 2 : 1);
            break;
        }
        if (moves == 9) {
            println("It's a draw!");
            break;
        }

        // Player input
        char mark = (player == 1) ? 'X' : 'O';
        println("Player %d, enter your choice: ", player);
        scanf("%d", &choice);

        // Update the board
        if (choice >= 1 && choice <= 9 && cells[choice - 1] != 'X' && cells[choice - 1] != 'O') {
            cells[choice - 1] = mark;
            player = (player == 1) ? 2 : 1;
            moves++;
        } else {
            println("Invalid move, try again.");
        }
    }

    // Free the allocated memory
    free(cells);
}