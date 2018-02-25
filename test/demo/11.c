#include "seahorn/seahorn.h"
extern int unknown1();

void main() {
  int x,m;
  x = 0;
  m = 0;

  int n = unknown1();

  while( x < n ) {
    if(unknown1())
	m = x;
	x++;
  }
  if( n > 0 )
    {
      sassert( 0<=m && m < n);
    }
}