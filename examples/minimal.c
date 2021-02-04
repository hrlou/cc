int max(int x,int y,int z) {
    if (x <= y) { 
        x = y;
    }
    if (x <= z) {
        x = z;
    }
    return x;
}

int main() {
    int x = max(17, 12, 32);
    printf("Highest Value: %d", x);
    if (x == 11) {
        return 17;
    }
    putchar('\n');
    return x;
}
