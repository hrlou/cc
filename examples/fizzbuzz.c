#include <stdio.h>
// Hello

int main() {
    for (int i = 1; i <= 100; i++) {
        if (i % 3 == 0) {
            fputs("Fizz", stdout);
        }
        if (i % 5 == 0) {
            fputs("Buzz", stdout);
        }
        printf("%d\n", i);
    }
    return 0;
}
