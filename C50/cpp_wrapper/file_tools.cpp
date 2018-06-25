#include <fstream>
#include <list>
#include <string>
#include <sstream>

#include <assert.h>
#include <stddef.h>

extern "C" {
#include "file_tools.h"
#include "clist.h"
}

int read_implications(const char * filename, cmap * map) {

	if(filename == NULL || map == NULL) {
		return false;
	}

	// Open file
	std::ifstream infile;
	infile.open(filename);

	if(!infile.is_open()) {
		return false;
	}

	// Read line by line
	std::string line;
	while (std::getline(infile, line)) {

		// Comments
		if(line.size() == 0 || line[0] == '#') {
			continue;
		}

		// Read line
		std::istringstream iss(line);
		int x, y;
		if ((iss >> x >> y)) {

			// Add to map
			cmap_add(map, x, y);

		}

	}

	// Close file
	infile.close();

	return true;

}


int read_intervals(const char * filename, clist * lower, clist * upper) {

	if(filename == NULL) {
		return 0;
	}	

		// Open file
	std::ifstream infile;
	infile.open(filename);

	if(!infile.is_open()) {
		return 0;
	}


	// Read line by line
	std::string line;
	std::list<int> bounds;
	while (std::getline(infile, line)) {

		// Comments
		if(line.size() == 0 || line[0] == '#') {
			continue;
		}

		// Read line
		std::istringstream iss(line);
		int x, y;
		if ((iss >> x >> y)) {

			// Add to upper and lower
			clist_push_back (lower, x);
			clist_push_back (upper, y);

		}

	}

	// Close file
	infile.close();

	assert (clist_size (lower) == clist_size (upper));

	return clist_size (lower);

}

