int max(int x,int y,int z) {
    if ( x <= y ) { 
        x = y;
    }
    if ( x <= z ) {
        x = z;
    }
    return x;
}

/* comment 1 */
// comment 2

int main() {
    int x = max(17, 12, 32);
    printf("Highest Value: %d\n", x);
    if (x == 11) {
        return 17;
    }
    return x;
}
