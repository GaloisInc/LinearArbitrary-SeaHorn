int main() {
	int n = unknown();
	int k, i, j;

	for (k=0,i=0; i<n; i++,k++);
		for (j=n; j>0; j--,k--) {
			if(k <= 0)
				__VERIFIER_error();
	}
	return 0;
}
