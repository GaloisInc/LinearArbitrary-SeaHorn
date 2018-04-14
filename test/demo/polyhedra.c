#include "seahorn/seahorn.h"
int main () {
	int x=1;
	int y=2*x;
	int n = unknown();
	while(x<=n){
		x=x+1;
		y=y+2*x;
	}
	sassert (y > x);
}