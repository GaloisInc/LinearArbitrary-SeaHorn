#include "defns.i"
#include "extern.i"

int verify (Tree tree, DataRec * cases, struct cmap * implications, int maxCase) {

	/*
	 * 1) Verify all that all cases are classified correctly
	 */
	int i;
	int total_cases_checked = 0;
	int total_positive_cases_checked = 0;
	int total_negative_cases_checked = 0;
	int classified_cases_checked = 0;
	ForEach (i, 0, maxCase) {

		// Check if defined
		//if (!Unknown (cases[i], ClassAtt)) {
		if (DVal(cases[i], ClassAtt) > 0) {

			assert (DVal(cases[i], ClassAtt) == Class(cases[i]));
			classified_cases_checked += 1;

			if (DVal(cases[i], ClassAtt) == 1) {
				total_positive_cases_checked += 1;
			} else if (DVal(cases[i], ClassAtt) == 2) {
				total_negative_cases_checked += 1;
			}

			if (TreeClassify (cases[i], tree) != DVal(cases[i], ClassAtt)) {

				printf ("Tree classification of case %d does not match table classification!\n", i);
				return 0;

			}

		}

	}
	total_cases_checked = i;

	/*
	 * 2) Verify whether implications are satisfied
	 */
	int ok = 1;
	int total_implications_checked = 0;
	int truetrue_implications_checked = 0;
	int falsetrue_implications_checked = 0;
	int falsefalse_implications_checked = 0;

	// Check every key
	struct array * keys = cmap_get_keys (implications);
	for (i=0; i<keys->size; i++) {

		struct array * values = cmap_get (implications, keys->entries[i]);

		int j;
		for (j=0; j<values->size; j++) {

			// Get classification of lhs and rhs
			ClassNo lhs_class = TreeClassify (cases[keys->entries[i]], tree);
			ClassNo rhs_class = TreeClassify (cases[values->entries[j]], tree);

			total_implications_checked += 1;

			if (lhs_class == 1 && rhs_class == 1) {
				truetrue_implications_checked += 1;
			} else if (lhs_class == 2 && rhs_class == 1) {
				falsetrue_implications_checked += 1;
			} else if (lhs_class == 2 && rhs_class == 2) {
				falsefalse_implications_checked += 1;
			}


			if (!(lhs_class != 1 || rhs_class == 1)) {

				printf ("Implications %d -> %d is not satisfied!\n", keys->entries[i], values->entries[j]);
				ok = 0;
				break;

			}

		}

		delete_array (values);

		if (!ok) {
			break;
		}

	}

	delete_array (keys);

	// Output
	if (ok) {
		printf ("Tree passed %d total data checks (%d positive, %d negative; %d in total)\n", total_cases_checked, total_positive_cases_checked, total_negative_cases_checked, classified_cases_checked);	
		printf ("Tree passed %d total implication checks (%d 1->1; %d 2->1; %d 2->2)\n", total_implications_checked, truetrue_implications_checked, falsetrue_implications_checked, falsefalse_implications_checked);
	

    		printf("filename: %s\n", FileStem);
		
		FILE *F = GetFile(".out", "w+");
		assert(F != NULL);
		fprintf(F, "%d\n", truetrue_implications_checked);
		fprintf(F, "%d\n", falsetrue_implications_checked);
		fprintf(F, "%d\n", falsefalse_implications_checked);
		fclose(F);
	}

	return ok;

}


int are_equal (DataRec * c1, DataRec * c2, int max_case, int max_attribute) {

	int i;
	ForEach (i, 0, max_case) {
		int j;
		ForEach (j, 0, max_attribute) {
			if (memcmp (&c1[i][j], &c2[i][j], sizeof (AttValue)) != 0) {
				return 0;
			}
		}
	}

	return 1;

}
