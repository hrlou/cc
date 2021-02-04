#include <stdio.h>

void main() {
    for (int i = 1; i <= 100; i++) {
        if (i % 3 == 0) {
            fputs("Fizz", stdout);
        } else if (i % 5 == 0) {
            fputs("Buzz", stdout);
        } else {
            printf("%d", i);
        }
        putchar('\n');
    }
}