#include "seahorn/seahorn.h"

extern int unknown1();

void main()
{
  int n, m;
  n = unknown1 ();
  m = unknown1 ();

  int x=0; 
  int y=m;

  if (n >= 0 && m >= 0 && m < n) {
  while(x<n) {
    x++;
    if(x>m) y++;
  }
  sassert(y==n);
  }
}
