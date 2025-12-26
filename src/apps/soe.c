#include "../stdlib/stdlib.h"

void dive(int n) {
    volatile char pad[4096];
    for (int i = 0; i < 4096; i++) pad[i] = 0;

    printf("Level %d down\n", n);

    if (n > 0) {
        dive(n - 1);
    }

    // <-- Добавь действие ПОСЛЕ вызова
    printf("Level %d up\n", n);
}

int main() {
    n_rec(10000);
}
