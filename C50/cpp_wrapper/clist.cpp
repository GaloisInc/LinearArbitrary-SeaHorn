#include <list>
#include <iostream>
#include <sstream>

#include <stddef.h>

extern "C" {
#include "clist.h"
#include "array.h"
}

struct clist {
	std::list<int> list;
};


clist * new_clist () {
	return new clist;
}

void delete_clist(clist * l) {
	if(l != NULL) {
		delete l;
	}
}

size_t clist_size (clist * l) {
	return l->list.size();
}

int clist_empty (clist * l) {
	return l->list.empty();
}

int clist_front (clist * l) {
	return l->list.front();
}

int clist_back (clist * l) {
	return l->list.back();
}

void clist_push_front (clist * l, int val) {
	l->list.push_front(val);
}

void clist_push_back (clist * l, int val) {
	l->list.push_back(val);
}

void clist_pop_front(clist * l) {
	l->list.pop_front();
}

void clist_pop_back(clist * l) {
	l->list.pop_back();
}

void clist_print(clist * l) {

	if(l == NULL) {
		std::cout << "NULL" << std::endl;
		return;
	}

	std::stringstream out;	

	out << "[";

	size_t i;
	std::list<int>::iterator it;
	for(i = 0, it = l->list.begin(); it != l->list.end(); it++, i++) {
		out << *it << (i < l->list.size() - 1 ? ", " : "");
	}

	out << "]";

	std::cout << out.str() << std::endl;

}

array * clist_to_array (clist const * l) {

	if (l == NULL) {
		return NULL;
	}

	array * a = new_array (l->list.size());

	int i=0;
	for (std::list<int>::const_iterator it = l->list.begin(); it != l->list.end(); it++) {

		a->entries[i] = *it;
		i++;

	}

	return a;

}
