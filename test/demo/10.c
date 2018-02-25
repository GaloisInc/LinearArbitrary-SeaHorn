#include "seahorn/seahorn.h"
extern int unknown1();

int main() {
   int b = 0;
   int j = 0;
   int x = unknown1();
   int bound = unknown1();

   __VERIFIER_assume(bound >= 10);

   for (b=0; b < bound ; ++b){
      if (x == 10)
         j = j +1;
   }


   if(x == 10)
      sassert(j==bound);
}