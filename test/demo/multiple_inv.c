#include "seahorn/seahorn.h"

int main()
{
  int k=0, j=0;
  int bound = unknown();
  
  while (j < bound) {
    j++;
    k++;
  }

  j=0;
  while (j < bound) {
    j++;
    k++;
  }
  
  j=0;
  while (j < bound) {
    j++;
    k++;
  }

  sassert(k>=3*bound);

  return 0;
}
