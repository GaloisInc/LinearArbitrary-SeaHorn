int main () {
	int x = 0; 
	int y = unknown();
	while (unknown())
		x += y ;
	
	while (unknown())
		y += x ;

	if (x > 0 && y < 0) 
		__VERIFIER_error();
}