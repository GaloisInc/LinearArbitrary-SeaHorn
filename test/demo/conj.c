#include "seahorn/seahorn.h"

int main()
{
	int x = unknown();
	__VERIFIER_assume((x > 0) && (x < 50));
	int flag = 1;
	while (unknown()) {
		x = x + flag;
		if (x >= 50) {
			x -= 2;
			flag = -1;
		}
		else if (x <= 0) {
			x += 2;
			flag = 1;
		}
	}
	sassert((x > 0) && (x < 50));
	return 0;
}
