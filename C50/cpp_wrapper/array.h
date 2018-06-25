#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stddef.h>

/*
 * Implements a set of integers, realized as an array of ints and its size.
 *
 * An empty array has size 0 and entries = NULL; thus, always check size first
 * before accessing the entries. 
 */ 
struct array {
	int * entries;
	size_t size;
};

/**
 * Creates a new set of given size.
 * Note that all entries are initialized using calloc.
 */ 
struct array * new_array(size_t);

// Frees the memory used by this array
void delete_array(struct array *);

// Create a new array that is an inverse of a
struct array * array_invert(struct array const *);

// Print the array to standard out
void array_print(struct array *);

#endif
