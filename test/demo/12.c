#include "seahorn/seahorn.h"
int main(void){
      int k=8;
      int j=34;
      int i=67;
      while(k<i) {
          k += 2;
          i++;
      j++;
      }
      sassert(k<=j);
}