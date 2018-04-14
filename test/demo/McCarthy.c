/*
 * Implementation the McCarthy 91 function.
 * http://en.wikipedia.org/wiki/McCarthy_91_function
 * 
 * Author: Matthias Heizmann
 * Date: 2013-07-13
 * 
 */
#include "seahorn/seahorn.h"

extern int __VERIFIER_nondet_int(void);


int f91(int x) {
    if (x > 100)
        return x - 10;
    else {
        return f91(f91(x+11));
    }
}


int main() {
    int x = __VERIFIER_nondet_int();
    int result = f91(x);
    sassert((result == 91 || x <= 90 || x >= 101) && (x < 101 || result == x - 10));
    sassert((result == 91 || x <= 80 || x >= 101) && (x < 101 || result == x - 10));
    sassert((result == 91 || x <= 70 || x >= 101) && (x < 101 || result == x - 10));
    sassert((result == 91 || x <= 60 || x >= 101) && (x < 101 || result == x - 10));
    sassert((result == 91 || x <= 50 || x >= 101) && (x < 101 || result == x - 10));
    sassert((result == 91 || x <= 40 || x >= 101) && (x < 101 || result == x - 10));
    sassert((result == 91 || x <= 30 || x >= 101) && (x < 101 || result == x - 10));
    sassert((result == 91 || x <= 20 || x >= 101) && (x < 101 || result == x - 10));
    sassert((result == 91 || x <= 10 || x >= 101) && (x < 101 || result == x - 10));
    sassert((result == 91 || x <= 0 || x >= 101) && (x < 101 || result == x - 10));
}
