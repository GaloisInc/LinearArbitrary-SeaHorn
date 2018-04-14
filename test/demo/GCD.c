#include "seahorn/seahorn.h"

extern int __VERIFIER_nondet_int(void);

// Compute the greatest common denominator using Euclid's algorithm
int gcd(int y1, int y2) {
    if (y1 <= 0 || y2 <= 0) {
        // No non-positive input allowed
        sassert(0);
    }
    if (y1 == y2) {
        return y1;
    }
    if (y1 > y2) {
        return gcd(y1 - y2, y2);
    }
    return gcd(y1, y2 - y1);
}

// does n divide m?
int divides(int n, int m) {
    if (m == 0) {
        return 1; // true
    }
    if (n > m) {
        return 0; // false
    }
    return divides(n, m - n);
}

int main() {
    int m = __VERIFIER_nondet_int();
    int n = __VERIFIER_nondet_int();
    if (m > 0 && n > 0 && m < 9 && n < 9) {
        int z = gcd(m, n);
        sassert (z > 0);
        int res = divides (z, m);
        sassert (res == 1);
    }
}
