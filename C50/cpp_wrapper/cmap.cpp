#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>

#include <stddef.h>
#include <assert.h>

extern "C" {
#include "cmap.h"
}

struct cmap {
	std::map<int, std::set<int> * > left2right_map; // Map from left to right
	std::map<int, std::set<int> * > right2left_map; // Map from right to left
};

cmap * new_cmap () {
	return new cmap;
}

void delete_cmap (cmap * map) {

	if(map != NULL) {

		for(std::map<int, std::set<int> * >::iterator it = map->left2right_map.begin(); it != map->left2right_map.end(); it++) {
			delete it->second;
		} 

		for(std::map<int, std::set<int> * >::iterator it = map->right2left_map.begin(); it != map->right2left_map.end(); it++) {
			delete it->second;
		} 

		delete map;
	}
}


cmap * cmap_copy (cmap const * other) {

	if (other == NULL) {
		return NULL;
	}

	cmap * copy = new cmap;

	// Copy left2right map
	for (std::map<int, std::set<int> *>::const_iterator it1 = other->left2right_map.begin(); it1 != other->left2right_map.end(); it1++) {

		if (it1->second == NULL) {
			copy->left2right_map[it1->first] = NULL;
		} else {
			copy->left2right_map[it1->first] = new std::set<int>(*(it1->second));
		}

	}

	// Copy right2left map
	for (std::map<int, std::set<int> *>::const_iterator it1 = other->right2left_map.begin(); it1 != other->right2left_map.end(); it1++) {

		if (it1->second == NULL) {
			copy->right2left_map[it1->first] = NULL;
		} else {
			copy->right2left_map[it1->first] = new std::set<int>(*(it1->second));
		}

	}

	return copy;

}


cmap * cmap_copy_and_rename (cmap const * other, array const * permutation) {

	if (other == NULL || permutation == NULL) {
		return NULL;
	}

	cmap * copy = new cmap;

	// Copy left2right map
	for (std::map<int, std::set<int> *>::const_iterator it1 = other->left2right_map.begin(); it1 != other->left2right_map.end(); it1++) {

		assert (0 <= it1->first && it1->first < permutation->size);

		if (it1->second == NULL) {
			copy->left2right_map[permutation->entries[it1->first]] = NULL;
		} else {

			std::set<int> * tmp = new std::set<int>;
			for (std::set<int>::const_iterator it2 = it1->second->begin(); it2 != it1->second->end(); it2++) {

				assert(0 <= *it2 && *it2 < permutation->size);
				tmp->insert(permutation->entries[*it2]);

			}
			copy->left2right_map[permutation->entries[it1->first]] = tmp;

		}

	}

	// Copy right2left map
	for (std::map<int, std::set<int> *>::const_iterator it1 = other->right2left_map.begin(); it1 != other->right2left_map.end(); it1++) {

		assert (0 <= it1->first && it1->first < permutation->size);

		if (it1->second == NULL) {
			copy->right2left_map[permutation->entries[it1->first]] = NULL;
		} else {

			std::set<int> * tmp = new std::set<int>;
			for (std::set<int>::const_iterator it2 = it1->second->begin(); it2 != it1->second->end(); it2++) {

				assert(0 <= *it2 && *it2 < permutation->size);
				tmp->insert(permutation->entries[*it2]);

			}
			copy->right2left_map[permutation->entries[it1->first]] = tmp;

		}

	}

	return copy;

}


void cmap_add (cmap * map, int x, int y) {

	// Insert y 
	std::map<int, std::set<int> *>::iterator it = map->left2right_map.find(x);
	if(it == map->left2right_map.end()) {
		
		std::set<int> * s = new std::set<int>;
		s->insert(y);
		map->left2right_map.insert(std::pair<int, std::set<int> *>(x, s));

	} else {

		assert(it->second != NULL);
		it->second->insert(y);

	}

	// Insert x
	it = map->right2left_map.find(y);
	if(it == map->right2left_map.end()) {

		std::set<int> * s = new std::set<int>;
		s->insert(x);
		map->right2left_map.insert(std::pair<int, std::set<int> *>(y, s));

	} else {

		assert(it->second != NULL);
		it->second->insert(x);

	}

}

void cmap_remove (cmap * map, int x, int y) {

	std::map<int, std::set<int> *>::iterator it = map->left2right_map.find(x);
	if(it != map->left2right_map.end()) {

		it->second->erase(y);

		if(it->second->size() == 0) {
			map->left2right_map.erase(x);
		}

	}

	it = map->right2left_map.find(y);
	if(it != map->right2left_map.end()) {

		it->second->erase(x);

		if(it->second->size() == 0) {
			map->right2left_map.erase(y);
		}

	}

}


array * cmap_get(cmap * map, int x) {
	
	std::map<int, std::set<int> *>::iterator it1 = map->left2right_map.find(x);
	if(it1 != map->left2right_map.end() && it1->second != NULL && it1->second->size() > 0) {

		array * a = new_array(it1->second->size());
		unsigned int i = 0;
		for(std::set<int>::iterator it2 = it1->second->begin(); it2 != it1->second->end(); it2++, i++) {
			a->entries[i] = *it2;
		}

		return a;

	} else {

		return new_array(0);

	}

}


array * cmap_rget(cmap * map, int y) {
	
	std::map<int, std::set<int> *>::iterator it1 = map->right2left_map.find(y);
	if(it1 != map->right2left_map.end() && it1->second != NULL && it1->second->size() > 0) {

		array * a = new_array(it1->second->size());
		unsigned int i = 0;
		for(std::set<int>::iterator it2 = it1->second->begin(); it2 != it1->second->end(); it2++, i++) {
			a->entries[i] = *it2;
		}

		return a;

	} else {

		return new_array (0);

	}

}


struct array * cmap_get_keys (struct cmap const * map) {

	if (map == NULL || map->left2right_map.size () == 0) {
		return new_array (0);
	}

	array * a = new_array (map->left2right_map.size ());
	int i=0;
	for (std::map<int, std::set<int> * >::const_iterator it = map->left2right_map.begin(); it != map->left2right_map.end(); it++) {

		a->entries[i] = it->first;
		i += 1;

	}

	return a;
	
}


void cmap_swap (cmap * map, int x, int y) {

	if(map == NULL) {
		return;
	}

	if(x == y) {
		return;
	}

	/*
	 * Get elements on both the mapping from left to right and vice versa
	 */
	std::map<int, std::set<int> *>::iterator l2r_xit = map->left2right_map.find(x);
	bool l2r_hasx = l2r_xit != map->left2right_map.end(); 
	std::map<int, std::set<int> *>::iterator l2r_yit = map->left2right_map.find(y);
	bool l2r_hasy = l2r_yit != map->left2right_map.end();
	std::map<int, std::set<int> *>::iterator r2l_xit = map->right2left_map.find(x);
	bool r2l_hasx = r2l_xit != map->right2left_map.end();
	std::map<int, std::set<int> *>::iterator r2l_yit = map->right2left_map.find(y);
	bool r2l_hasy = r2l_yit != map->right2left_map.end();

	// Skip if nothing is to be renamed
	if(!l2r_hasx && !l2r_hasy && !r2l_hasx && !r2l_hasy) {
		return;
	}


	/*
	 * Update left2right mapping
	 */

	// Update left hand side
	// 1. case: both x and y are present
	if(l2r_hasx && l2r_hasy) {

		std::set<int> * tmp = l2r_xit->second;
		map->left2right_map[x] = l2r_yit->second;
		map->left2right_map[y] = tmp;

	}

	// 2. case: x is the only contained element
	else if(l2r_hasx) {

		std::set<int> * tmp = l2r_xit->second;
		map->left2right_map.erase(x);
		map->left2right_map[y] = tmp;

	}

	// 3. case: y is the only contained element
	else if(l2r_hasy) {

		std::set<int> * tmp = l2r_yit->second;
		map->left2right_map.erase(y);
		map->left2right_map[x] = tmp;

	}


	// Update right hand side
	if(r2l_hasx || r2l_hasy) {

		for(std::map<int, std::set<int> *>::iterator it = map->left2right_map.begin(); it != map->left2right_map.end(); it++) {

			bool has_x = it->second->count(x) > 0;
			bool has_y = it->second->count(y) > 0;

			if(has_x && !has_y) {
		
				it->second->erase(x);
				it->second->insert(y);

			} else if(!has_x && has_y) {
		
				it->second->erase(y);
				it->second->insert(x);

			}

		}

	}


	/*
	 * Update right2left mapping
	 */

	// Update left hand side
	// 1. case: both x and y are present
	if(r2l_hasx && r2l_hasy) {

		std::set<int> * tmp = r2l_xit->second;
		map->right2left_map[x] = r2l_yit->second;
		map->right2left_map[y] = tmp;

	}

	// 2. case: x is the only contained element
	else if(r2l_hasx) {

		std::set<int> * tmp = r2l_xit->second;
		map->right2left_map.erase(x);
		map->right2left_map[y] = tmp;

	}

	// 3. case: y is the only contained element
	else if(r2l_hasy) {

		std::set<int> * tmp = r2l_yit->second;
		map->right2left_map.erase(y);
		map->right2left_map[x] = tmp;

	}


	// Update right hand side
	if(l2r_hasx || l2r_hasy) {

		for(std::map<int, std::set<int> *>::iterator it = map->right2left_map.begin(); it != map->right2left_map.end(); it++) {

			bool has_x = it->second->count(x) > 0;
			bool has_y = it->second->count(y) > 0;

			if(has_x && !has_y) {
		
				it->second->erase(x);
				it->second->insert(y);

			} else if(!has_x && has_y) {
		
				it->second->erase(y);
				it->second->insert(x);

			}

		}

	}


}

// TODO: Remove points from the implications when the implication constraint no longer remains after propagation?
int cmap_propagate (array * a, cmap * m, int classification, int (*callback)(int, int)) {

	if (a == NULL || m == NULL || callback == NULL) {
		return 0;
	}

	// Select map depending on what kind of classification
	std::map<int, std::set<int> *> * implications = NULL;
	if (classification == 1) {
		implications = &(m->left2right_map);
	} else if (classification == 2) {
		implications = &(m->right2left_map);
	} else {
		assert(false);
	}

	// Prepare data structures
	std::set<int> transitive_closure;
	std::list<int> queue;
	
	// initialize data structures
	for (int i=0; i<a->size; i++) {
		queue.push_back(a->entries[i]);
	}

	// Compute transitive closure
	while (!queue.empty()) {

		// Get current element
		int cur = queue.front();
		queue.pop_front();
		
		// Process current element if not yet done so
		if (transitive_closure.count(cur) == 0) {

			transitive_closure.insert(cur);
			int proceed = callback(cur, classification);

			// Process successors if proceed is true
			if ( proceed )
			{
				std::map<int, std::set<int> *>::iterator it1 = implications->find(cur);
				if (it1 != implications->end()) {

					assert(it1->second != NULL);
					for (std::set<int>::iterator it2 = it1->second->begin(); it2 != it1->second->end(); it2++) {
						queue.push_back(*it2);
					}

				}
			}

		}

	};

	return transitive_closure.size();

}


int cmap_count_cuts (struct cmap const * m, struct array const * a1, struct array const * a2) {

	if (m == NULL || a1 == NULL || a2 == NULL) {
		return -1;
	}

	// Copy arrays into sets for faster access
	std::set<int> s1, s2;
	for (int i = 0; i < a1->size; i++) {
		s1.insert(a1->entries[i]);
	}
	for (int i = 0; i < a2->size; i++) {
		s2.insert(a2->entries[i]);
	}

	// Now count
	int count = 0;
	for (std::map<int, std::set<int> *>::const_iterator it1 = m->left2right_map.begin(); it1 != m->left2right_map.end(); it1++) {
		for (std::set<int>::const_iterator it2 = it1->second->begin(); it2 != it1->second->end(); it2++) {

			if (s1.count(it1->first) > 0 && s2.count(*it2) > 0) {
				count++;
			} else if(s2.count(it1->first) > 0 && s1.count(*it2) > 0) {
				count++;
			}

		}
	}

	return count;

}

array * __select (std::map<int, std::set<int> * > const & map, int (*callback) (int, int)) {

	// Select
	std::list<int> result;
	for (std::map<int, std::set<int> * >::const_iterator it1 = map.begin(); it1 != map.end(); it1++) {
		for (std::set<int>::const_iterator it2 = it1->second->begin(); it2 != it1->second->end(); it2++) {

			if (callback (it1->first, *it2)) {
				result.push_back (*it2);
			}

		}
	} 

	// Convert to array
	array * a = new_array (result.size ());
	int i=0;
	for (std::list<int>::const_iterator it = result.begin(); it != result.end(); it++) {

		a->entries[i] = *it;
		i++;

	}

	return a;

}

array * cmap_select_lhs (struct cmap * map, int (*callback) (int, int)) {

	if (map == NULL || callback == NULL) {
		return NULL;
	} else {
		return __select(map->right2left_map, callback);
	}

}
array * cmap_select_rhs (struct cmap * map, int (*callback) (int, int)) {

	if (map == NULL || callback == NULL) {
		return NULL;
	} else {
		return __select(map->left2right_map, callback);
	}

}


int cmap_number_of_implications (cmap const * map) {

	if (map == NULL) {
		return -1;
	}

	int number = 0;
	for (std::map<int, std::set<int> *>::const_iterator it1 = map->left2right_map.begin(); it1 != map->left2right_map.end(); it1++) {

		if (it1->second != NULL) {

			for (std::set<int>::const_iterator it2 = it1->second->begin(); it2 != it1->second->end(); it2++) {
				number += 1;
			}

		}

	}

	return number;

}


void cmap_print (cmap * map) {


	if(map == NULL) {
		std::cout << "NULL" << std::endl;
		return;
	}


	std::stringstream out;
	out << "left 2 right:";

	for(std::map<int, std::set<int> *>::iterator it1 = map->left2right_map.begin(); it1 != map->left2right_map.end(); it1++) {
		out << std::endl << "\t" << it1->first << " -> [";

		std::set<int>::iterator it2 = it1->second->begin();
		unsigned int i=0;
		while(it2 != it1->second->end()) {

			out << *it2;
			if(i+1 < it1->second->size()) {
				out << ", ";
			}

			i++;
			it2++;

		}

		out << "]";

	}

	out << std::endl << "right 2 left:";
	for(std::map<int, std::set<int> *>::iterator it1 = map->right2left_map.begin(); it1 != map->right2left_map.end(); it1++) {
		out << std::endl << "\t" << it1->first << " <- [";

		std::set<int>::iterator it2 = it1->second->begin();
		unsigned int i=0;
		while(it2 != it1->second->end()) {

			out << *it2;
			if(i+1 < it1->second->size()) {
				out << ", ";
			}

			i++;
			it2++;

		}

		out << "]";

	}

	std::cout << out.str() << std::endl;

}

/*
Returns the number of implications that start at i - implications that end at i.
Only consider those implications that lie entirely between Fp and Lp.
*/
int numImplicationsCutAt(int i, int Fp, int Lp, struct cmap const * Implications, struct array const * classAttr)
{
    int ret = 0;

    std::map<int, std::set<int> * >::const_iterator mit = Implications->left2right_map.find(i);
    if (mit != Implications->left2right_map.end())
    {
	if (classAttr->entries[i] == 0)
	{
	    for(std::set<int>::const_iterator sit = mit->second->begin(); sit != mit->second->end(); sit++)
     	    {
	    	assert (*sit != i);
	    	if ((*sit > i) && (*sit <= Lp) && classAttr->entries[*sit] == 0)
	 	    ret += 1;

	    	else if ((*sit < i) && (*sit >= Fp) && classAttr->entries[*sit] == 0)
		    ret -= 1;
	    }
	}
    }

    mit = Implications->right2left_map.find(i);
    if (mit != Implications->right2left_map.end())
    {
	if (classAttr->entries[i] == 0)
	{
	    for(std::set<int>::const_iterator sit = mit->second->begin(); sit != mit->second->end(); sit++)
     	    {
	    	assert (*sit != i);
	    	if ((*sit > i) && (*sit <= Lp) && classAttr->entries[*sit] == 0)
	 	    ret += 1;

	    	else if ((*sit < i) && (*sit >= Fp) && classAttr->entries[*sit] == 0)
		    ret -= 1;
	    }
	}
    }

    return ret;
}

/*
Assigns in the pointer arguments the number of implications that start at i - implications that end at i.
Only consider those implications that lie entirely between Fp and Lp. Handles implications from right to 
left and left to right separately.
*/
void numImplicationsCutAtLeftRightSep(int i, int Fp, int Lp, struct cmap const * Implications, struct array const * classAttr, 
					int *numImplicationsCutLeft2Right, int *numImplicationsCutRight2Left)
{
    int ret = 0;

    std::map<int, std::set<int> * >::const_iterator mit = Implications->left2right_map.find(i);
    if (mit != Implications->left2right_map.end())
    {
	if (classAttr->entries[i] == 0)
	{
	    for(std::set<int>::const_iterator sit = mit->second->begin(); sit != mit->second->end(); sit++)
     	    {
	    	assert (*sit != i);
	    	if ((*sit > i) && (*sit <= Lp) && classAttr->entries[*sit] == 0)
		    *numImplicationsCutLeft2Right = *numImplicationsCutLeft2Right + 1;

	    	else if ((*sit < i) && (*sit >= Fp) && classAttr->entries[*sit] == 0)
		    *numImplicationsCutRight2Left = *numImplicationsCutRight2Left - 1;
	    }
	}
    }

    mit = Implications->right2left_map.find(i);
    if (mit != Implications->right2left_map.end())
    {
	if (classAttr->entries[i] == 0)
	{
	    for(std::set<int>::const_iterator sit = mit->second->begin(); sit != mit->second->end(); sit++)
     	    {
	    	assert (*sit != i);
	    	if ((*sit > i) && (*sit <= Lp) && classAttr->entries[*sit] == 0)
		    *numImplicationsCutRight2Left = *numImplicationsCutRight2Left + 1;

	    	else if ((*sit < i) && (*sit >= Fp) && classAttr->entries[*sit] == 0)
		    *numImplicationsCutLeft2Right = *numImplicationsCutLeft2Right - 1;
	    }
	}
    }

    return;
}



/*
Asigns *right2cut and *cut2left with the number of implications that have flown from the partition
consisting of points that fall to the right of the threshold "<i" to now being cut by the threshold at "i",
and similarly implications that have flown from being cut by the threshold "<i" to now being present entirely
in the left partition with threshold at "i".
Only consider those implications that lie entirely between Fp and Lp.
*/
void processImplicationsAt(int i, int Fp, int Lp, struct cmap const * Implications, struct array const * classAttr,
											int * right2cut, int * cut2left)
{
    *right2cut = 0;
    *cut2left = 0;

    std::map<int, std::set<int> * >::const_iterator mit = Implications->left2right_map.find(i);
    if (mit != Implications->left2right_map.end())
    {
	if (classAttr->entries[i] == 0)
	{
	    for(std::set<int>::const_iterator sit = mit->second->begin(); sit != mit->second->end(); sit++)
     	    {
	    	assert (*sit != i);
	    	if ((*sit > i) && (*sit <= Lp) && classAttr->entries[*sit] == 0)
	 	    *right2cut += 1; 

	    	else if ((*sit < i) && (*sit >= Fp) && classAttr->entries[*sit] == 0)
		    *cut2left += 1;
	    }
	}
    }

    mit = Implications->right2left_map.find(i);
    if (mit != Implications->right2left_map.end())
    {
	if (classAttr->entries[i] == 0)
	{
	    for(std::set<int>::const_iterator sit = mit->second->begin(); sit != mit->second->end(); sit++)
     	    {
	    	assert (*sit != i);
	    	if ((*sit > i) && (*sit <= Lp) && classAttr->entries[*sit] == 0)
	 	    *right2cut += 1;

	    	else if ((*sit < i) && (*sit >= Fp) && classAttr->entries[*sit] == 0)
		    *cut2left += 1;
	    }
	}
    }

    return; 
}


int computeTotalImplications(int Fp, int Lp, struct cmap const * Implications, struct array const * classAttr)
{
    int i, ret = 0;

    for(i = Fp; i <= Lp; i++)
    {
    	std::map<int, std::set<int> * >::const_iterator mit = Implications->left2right_map.find(i);
    	if (mit != Implications->left2right_map.end())
    	{
	    if (classAttr->entries[i] == 0)
	    {	

	    	for(std::set<int>::const_iterator sit = mit->second->begin(); sit != mit->second->end(); sit++)
     	    	{
		    assert (*sit != i);
	    	    if ((*sit > i) && (*sit <= Lp) && classAttr->entries[*sit] == 0)
	 	    	ret += 1;
	    	}
	    }
    	}

    	mit = Implications->right2left_map.find(i);
    	if (mit != Implications->right2left_map.end())
    	{
	    if (classAttr->entries[i] == 0)
	    {
	        for(std::set<int>::const_iterator sit = mit->second->begin(); sit != mit->second->end(); sit++)
     	    	{
		    assert (*sit != i);
	    	    if ((*sit > i) && (*sit <= Lp) && classAttr->entries[*sit] == 0)
	 	        ret += 1;
	    	}
	    }
    	}
    }

    return ret;
}


