#include "seahorn/seahorn.h"

int main() {
    int p, n;
    p = 42;
    n = unknown();
    while ( n>=0 ) {
        sassert (p != 0);
        if (n == 0) {
            p = 0;
        }
        n--;
    }
    return 0;
}