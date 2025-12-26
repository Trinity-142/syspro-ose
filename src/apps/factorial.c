#include "../stdlib/stdlib.h"

u32 factorial(int n) {
    if (n == 0) return 1;
    u32 a = n * factorial(n - 1);
    printf("Factorial of %d: %d\n", n, a);
    return a;
}

int main() {
    factorial(10);
}
