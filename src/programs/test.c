#include "test.h"
#include "../libs/stdio.h"


void test() {
    println("test of scanf");
    char str[100];
    int num = 0;
    printf("Enter a string: ");
    scanf("%s", str);
    printf("\nYou entered: %s\n", str);
    printf("Enter a number: ");
    scanf("%d", &num);
    printf("\nYou entered: %d\n", num);
}