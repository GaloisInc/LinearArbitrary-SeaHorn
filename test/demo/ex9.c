#include "seahorn/seahorn.h"

int fib(int n, int high) {
    int f1 = 1, f2 = 0, temp = 0;
    if (high) {
       while(n > 0) {
          f1 = f1 + f2;
          f2 = f1 - f2;
          n--;
}
} else {
       while(n > 0) {
          temp = f2;
          f2 = f1;
          f1 = f2 + temp;
          n--;
} }
return f1; }

void main() {
    int n1, n2, obs1, obs2, high1, high2;
    n1 = unknown();
    n2 = unknown();
    high1 = unknown();
    high2 = unknown();
    __VERIFIER_assume(n1 == n2);
    __VERIFIER_assume(0 <= n1 && n1 <= 5);
    // Note: high1 might not equal high2
    obs1 = fib(n1, high1);
    obs2 = fib(n2, high2);
    sassert(obs1 == obs2);
}