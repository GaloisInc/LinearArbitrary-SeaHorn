#include "seahorn/seahorn.h"

int g(int n)
{
  int r;
  r = 0;

  if (n <= 0) {
    r = 0;
  } else {
    r = g(n - 1);
    r = n + r;
  }

  return r;
}

int g1(int n, int s)
{
  int r;
  r = 0;

  if (n <= 0) {
    r = s;
  } else {
    r = g1(n - 1, n + s);
  }

  return r;
}

int triangle(int n) {
  int r;
  r = g(n);
  return r;
}

int triangle1(int n) {
  int r;
  r = g1(n, 0);
  return r;
}

int main () {
  int n = unknown();
  __VERIFIER_assume (n <= 9);
  sassert (triangle(n) == triangle1(n));
}