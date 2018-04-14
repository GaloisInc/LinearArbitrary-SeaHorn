int main () {
	int x, y;
	x = 0;
	y = unknown();
	__VERIFIER_assume (y >= 0);
	while (x < 10000) {
		y++;
		x++;
	}
	if (y + x < 10000) {
		__VERIFIER_error();
	}
}