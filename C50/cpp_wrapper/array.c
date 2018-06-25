#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "array.h"

struct array * new_array(size_t total_size) {

	struct array * ret = malloc(sizeof(struct array));
	ret->size = total_size;

	if(total_size == 0) {
		ret->entries = NULL;
	} else {
		ret->entries = calloc(total_size, sizeof(int));
	}

	return ret;
}


void delete_array(struct array * a) {
	
	if(a == NULL) return;

	free(a->entries);
	free(a);

}


struct array * array_invert(struct array const * a)
{
    int i;

    if (a == NULL)
	return NULL;

    struct array * inverse = new_array(a->size);
    for(i = 0; i < a->size; i++)
    {
	inverse->entries[a->entries[i]] = i;
    }

    return inverse;
}


void array_print(struct array * a) {

	printf("[");
	size_t i;
	for(i=0; i<a->size; i++) {

		printf("%d", a->entries[i]);
		if(i < a->size-1) {
			printf(", ");
		}

	}
	printf("]\n");

}
