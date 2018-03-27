#include "seahorn/seahorn.h"

int main() {
    int x, y, z, w;
    x = unknown();
    y = unknown();
    z = unknown();
    w = unknown();

    do {
        z = 0;
        x = y;
        if (w) {
            x++;
            z=1;
        }
    } while (x!=y);
    if (z) {
        sassert (0);
    }
}
