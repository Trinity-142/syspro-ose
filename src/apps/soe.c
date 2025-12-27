#include "../stdlib/stdlib.h"

void dive(int n) {
    volatile char pad[4096];
    for (int i = 0; i < 4096; i++) pad[i] = 0;
    if (n > 0) {
        dive(n - 1);
    }
    printf("%d\n", n);
}

int main() {
    //exit(52);
    //*((u32*)0x42) = 0;
    dive(10000);
}
