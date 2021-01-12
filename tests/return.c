#include <stdio.h>
#include <stdlib.h>

// deplorable syntax
int $_max(int x,int y,int z){if(x<=y){x=y;}if(x<=z){x=z;}return x;}
#define HELLO "Hello, World!\n"
int main() {
    #define _TEST
    /* this is a comment */
    int x = $_max(17, 12, 32);
    printf("Highest Value: %d\n", x);
    return x;
}
