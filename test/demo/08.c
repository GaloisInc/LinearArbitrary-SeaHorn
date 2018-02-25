#include "seahorn/seahorn.h"

extern int unknown1();

void main() {
  int x,y;

  x = -50;
  y = unknown1 ();
  while( x < 0 ) {
	x = x+y;
	y++;
  }
  sassert(y>0);
}