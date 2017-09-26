#include <stdio.h>

int somme(int a, int b) {
    return a + b;
}

int division(int a, int b) {
    return a / b;
}

void hello_world() {
    printf("Hello World!\n");
}

int main(int argc, char *argv[]) {
    int a=1, b=2;
    hello_world();
    somme(a, b);
    division(a, b);
    return 0;
}