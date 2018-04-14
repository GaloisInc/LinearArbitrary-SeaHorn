int main () {
	int s = 1;
	int x1 = 0;
	int x2 = 0;
	while (nondet ()) {
		if (s ==1) x1++;
		else if (s ==2) x2++;
		s++;
		if (s==5) s = 1;
		if (s==1 && x1 != x2 ) 
			__VERIFIER_error();
	}
}