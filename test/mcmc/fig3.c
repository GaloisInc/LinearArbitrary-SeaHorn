#include "seahorn/seahorn.h"

//pre: true
void main() {
	int y = unknown();
	int lock;
	lock = 0;
	int x;

	// Converting do-while to a while loop by stripping the first iteration
	{
		lock = 1;
		x = y;
		if( unknown() ) {

			lock = 0;
			y = y + 1;
		}
	}

	while(x != y) {

		lock = 1;
		x = y;
		if ( unknown() ) {

			lock = 0;
			y = y + 1;
		}
	}

	sassert(lock == 1);

}
//post: lock = 1
