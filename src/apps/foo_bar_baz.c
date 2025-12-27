#include "../stdlib/stdlib.h"
#include "../stdlib/types.h"

int main (int argc, char* argv[]) {
    for (u32 i = 1; i; i++) {
        printf("%s ", argv[i % argc]);
    }
}
