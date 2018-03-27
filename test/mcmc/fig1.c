#include "seahorn/seahorn.h"

//pre: true
void main() {
	int y = unknown();
	int x = -50;

 	while (x < 0) {
		x = x + y;
		y++;
	
	}
	sassert (y > 0);
}
