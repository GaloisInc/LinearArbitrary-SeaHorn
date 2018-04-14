#include "seahorn/seahorn.h"
int main () {
	int m = unknown();
	int sec = unknown();
	int c = unknown();
	int d, t, s;
	d = t = s = 0;
	__VERIFIER_assume(s <= c && c < 10);
	while(1)
	{
		if(sec) {
			t++; s = 0;
		}
		else if(m) {
			if(s < c) {
				d++; s++;
			}
		}
		sassert (0 <= s && s <= c && d >= s);
		sassert (d <= 10*t + s);
	}
}