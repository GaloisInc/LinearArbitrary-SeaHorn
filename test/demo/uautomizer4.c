#include "seahorn/seahorn.h"

int main() {
    int x, y;

    x = 0;
    y = 0;

    while (unknown()) {
        x = x + 1;
    }

    sassert (x != -1);
    sassert (y != -1);
    return 0;
}