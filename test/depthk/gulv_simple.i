#include "seahorn/seahorn.h"

int main(){
  int x=0;
  int y=0;
  int z=0;
  int w=0;

  while ( unknown() ){
    if ( unknown() ) {
      x++; y = y+100;
    } else if  ( unknown() ) {
      if( x >= 4)
        { x=x+1; y=y+1;}
    } 
  }
  sassert(x < 4 || y>2);
}
