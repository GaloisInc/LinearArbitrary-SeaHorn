#ifndef __FILE_TOOLS_H__
#define __FILE_TOOLS_H__

#include "array.h"
#include "cmap.h"
#include "clist.h"

/*
 * Reads implications from a file. The file is given as
 * as a string (\0 terminated). The return value is a
 * boolean indicating whether the operation was successful.
 *
 * The format is simple:
 * - Each line defines an implication of the form int int
 *   (separated by a blank)
 * - A line starting with # is ignored
 */
int read_implications(const char * filename, struct cmap *);

/*
 * Reads a set of intervals and returns an array with the upper intervals bounds.
 *
 */ 
int read_intervals(const char * filename, struct clist*, struct clist *); 

#endif
