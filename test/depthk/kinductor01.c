#include "seahorn/seahorn.h"

int rotate(int N)
{
  int a, b, c, x, i, temp;
  a = 1;
  b = 2;
  c = 3;
  x = 0;

  for(i = 0; i < N; i++)
  {
    //__CPROVER_k_induction_hint(3,3,0,0);
    sassert(a != b);
    temp = a;
    a = b;
    b = c;
    c = temp;
  }

  sassert(x == 0);

}

int main()
{
  rotate(unknown());  
  return 0;
}