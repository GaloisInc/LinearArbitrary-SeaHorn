#include "seahorn/seahorn.h"

void main(){
    int x = 0 ;
    int t = 0 ;
    int n = 100;
    int phase = 0 ;

    while ( t < n) {
        if( phase == 0 ){
            x = x + 2;
	}
        if( phase == 1 ){
            x = x - 1;
	}
        phase = 1 - phase;
        t++;
    }
    sassert( x <= n ); 
}