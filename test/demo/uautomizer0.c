#include "seahorn/seahorn.h"

int main() {
    int t = 0, d = 0, s = 0;
    int sec, met;
    sec = unknown();
    met = unknonw();
    while (nondet()) {
        if (sec) {
            s = 0;
            if (t++ == 4) {
                break;
            }
        }
        if (met) {
            if (s++ == 3) {
                break;
            }
            d++;
            sassert (d != 10 );
        }
    }
}