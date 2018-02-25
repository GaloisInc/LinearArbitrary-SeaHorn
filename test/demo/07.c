#include "seahorn/seahorn.h"

int main(int n){
  int x=0;

  if(n>0) {
	while(x<n){
    		x++;
  	}
  	sassert(x<=n);
  }
}