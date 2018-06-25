/*
 * A (basic) C wrapper for the C++ std::list<int> class.
 *
 * The methods defined here mirror those of the C++ class.
 */

#ifndef __CLIST_H__
#define __CLIST_H__

// Includes
#include "array.h"


// Forward declaration of the clist
struct clist;

struct clist * new_clist ();

void delete_clist (struct clist *);

size_t clist_size (struct clist *);

int clist_empty(struct clist *);

// Calling this function on an empty container causes undefined behavior.
int clist_front(struct clist *);

// Calling this function on an empty container causes undefined behavior.
int clist_back(struct clist *);

void clist_push_front(struct clist *, int);

void clist_push_back(struct clist *, int);

void clist_pop_front(struct clist *);

void clist_pop_back(struct clist *);

void clist_print(struct clist *);

// Copies the content of the list into an array
struct array * clist_to_array (struct clist const *);

#endif
