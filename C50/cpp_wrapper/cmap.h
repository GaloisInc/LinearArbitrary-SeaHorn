/*
 * A map f: int -> 2^int that provides access to both
 * - f(x); and
 * - for a given y, the set { x | y \in f(x) }.
 */

#ifndef __CMAP_H__
#define __CMAP_H__

// Includes
#include "array.h"

// Forward declaration of the map
struct cmap;

// Creates a new c_map
struct cmap * new_cmap ();

// Creates a copy of the given cmap
struct cmap * cmap_copy  (struct cmap const *);

// Creates a copy of the given map while renaming its entries
// w.r.t. the given permutation array.
struct cmap * cmap_copy_and_rename (struct cmap const *, struct array const *);

// Deletes a c_map
void delete_cmap  (struct cmap *);

// Adds extends or replaces the mapping with y \in f(x)   
void cmap_add (struct cmap *, int, int);

// Removes y from f(x)
void cmap_remove (struct cmap *, int, int);

// Returns f(x)
struct array * cmap_get (struct cmap *, int);

// Returns for a given y the set {x | y \in f(x)}
struct array * cmap_rget (struct cmap *, int);

// Returns all keys stored in the map
struct array * cmap_get_keys (struct cmap const *);

// Renames two entries
void cmap_swap (struct cmap *, int, int);

/*
 * Propagates a classification starting from a set of elements
 * along given implications. The third parameter specifies
 * the classification to set the elements to.
 *
 * Returns the number of elements processed.
 *
 * The third parameter is a callback function that is called
 * everytime a new element is processed. It takes two parameters:
 * 1. The current element (i.e., its index)
 * 2. The classification to set the element to (i.e., the third paramter of
 *    the method call is passed on)
 */
int cmap_propagate (struct array *, struct cmap *, int, int (*) (int, int));

/*
 * Collects all indices of elements on the left hand side of an implication
 * in the given map for which the function passed as second parameter evaluates
 * to true. Ths function takes two integers: the left hand side and the right
 * hand side of an implication.
 */
struct array * cmap_select_lhs (struct cmap *, int (*) (int, int));

/*
 * Same as above but selects from the right hand side.
 */
struct array * cmap_select_rhs (struct cmap *, int (*) (int, int));

// Returns the number of implications stored in the map
int cmap_number_of_implications (struct cmap const *);

/*
 * Computes the number of implications in a cmap that were to be cut
 * if entries are distributed into two sets (given as arrays).
 */
int cmap_count_cuts (struct cmap *, struct array *, struct array *);

// Prints the map
void cmap_print (struct cmap * map);


int numImplicationsCutAt(int i, int Fp, int Lp, struct cmap const * Implications, struct array const * classAttr);
void numImplicationsCutAtLeftRightSep(int i, int Fp, int Lp, struct cmap const * Implications, struct array const * classAttr,
							int *numImplicationsCutLeft2Right, int *numImplicationsCutRight2Left);

void processImplicationsAt(int i, int Fp, int Lp, struct cmap const * Implications, struct array const * classAttr, int * right2cut, int * cut2left);

int computeTotalImplications(int Fp, int Lp, struct cmap const * Implications, struct array const * classAttr);

#endif
