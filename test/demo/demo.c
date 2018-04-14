int main () {
	int c = unknown();
	int z = 0;
	if (c < 0) 
		return 0;
	
	while (nondet() && c < 10) {
		c = c + 1;
		z = z - c;
	}
	if (z == 10) {
		c = c + 2;
		z = z - c;
	}
	while (nondet() && c > 10) {
		c = c + 1;
		z = z - c;
	}
	if (z > 0) {
		__VERIFIER_error();
	}
	return 0;
}