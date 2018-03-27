#include "seahorn/seahorn.h"

void main()
{
int n = unknown();
if(n<=0) return;
int I, J;
for(I=0;I<n;I++)
  for(J=0; J<n;J++)
  {
    sassert( 0<=J && J<=n && 0<=I && I<=n);
  }
}
