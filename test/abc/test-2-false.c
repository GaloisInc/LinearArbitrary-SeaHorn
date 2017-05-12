// RUN: %sea pf -O0 --abc=%abc_encoding %dsa "%s" %abc3_definitions 2>&1 | OutputCheck %s
// CHECK: ^sat$

#include <stdio.h>
extern int nd ();

int main(int argc, char**argv) 
{
  int i;
  int a[10];
  for (i = 0; i < 10; i++) 
  {
    a[i+1] = 9999;
  }

  // trick llvm so that it cannot detect overflow
  printf("%d\n", a[(nd()>0?i-1:i)]);
  return 42;
}