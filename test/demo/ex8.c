void main() {
	int id1 = unknown();
	int id2 = unknown();
	int id3 = unknown();
	int max1=id1, max2=id2, max3=id3;
	int i=0, cnt=0;

	if(id1!=id2 && id1!=id3 && id2!=id3)
	while (1) {
		if (max3 > max1) max1 = max3;
		if (max1 > max2) max2 = max1;
		if (max2 > max3) max3 = max2;

		if (i == 1) {
			if (max1 == id1) cnt++;
			if (max2 == id2) cnt++;
			if (max3 == id3) cnt++;
		}

		if (i>=1) 
			if(cnt!=1)
				__VERIFIER_error();
		i++;
	}
}