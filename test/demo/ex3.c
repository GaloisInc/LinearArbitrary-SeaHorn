#include "seahorn/seahorn.h"
int main () {
	int x = unknown();
	int y = unknown();
	__VERIFIER_assume(x < 0);
	while(x < 0){
		x = x + y;
		y++;
	}
	sassert(y > 0);

	x = 1; 
	y = 0;

	while (unknown()) {
		x += y;
		y++;
	}
	sassert (x >= y);

	x = unknown();
	y = unknown();
	__VERIFIER_assume(x > 0 || y > 0);
	while(x + y <= -2){
		if (x > 0){
			x = x + 1;
		} else {
			y =y + 1;
		}
	}
	sassert(x > 0 || y > 0);

	x = unknown();
	y = unknown();
	__VERIFIER_assume(x < y);
	while(x < y){
		if (x < 0) x = x + 7;
		else x = x + 10;
		if (y < 0) y = y - 10;
		else y = y + 3;
	}
	sassert(y <= x && x <= y + 16);
}