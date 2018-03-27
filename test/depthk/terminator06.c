#include "seahorn/seahorn.h"

void main()
{
  int x, y, z;

  x = unknown();
  y = unknown();
  z = unknown();

  while (x>0 && y>0 && z>0)
  {
    int c1 = unknown();
    int c2 = unknown();
    if(c1) {
      x=x-1;
    } else if (c2) {
      y = y- 1;
      z=unknown();
    } else {
      z=z-1;
      x=unknown();
    }
  }                           
  sassert(!(x>0 && y>0 && z>0));    
}