#include <ostream>
#include <cstring>
#include <string>
#include <sstream>

extern "C" {
#include "defns.i"
#include "extern.i"

#include "tree_serializer.h"
}

/**
 * Translates a C5 decision tree into a JSON serialization.
 *
 * @param t The C5 decision tree to serialize
 * @param out An output stream to serialize to
 */
void _serialize_to_JSON (Tree t, std::ostream & out) {

	// We can only handle leafs, continuous attributes, or discrete attributes
	assert (!t->NodeType || (t->NodeType == BrThresh && Continuous (t->Tested)) || (t->NodeType == BrDiscr && Discrete (t->Tested)));
	
	// Start of class
	out << "{";

	// Inner node: continuous attribute
	if (t->NodeType == BrThresh) {

		// We can only handle two branches
		assert (t->Forks == 3);

		// Attribute
		out << "\"attribute\":\"" << AttName[t->Tested] << "\",";

		// Cut
		char some_char_array[20];
		CValToStr(t->Cut, t->Tested, some_char_array);
		out << "\"cut\":" << some_char_array << ",";

		// Classification
		out << "\"classification\":0,";

		// Children
		out << "\"children\":[";
		_serialize_to_JSON (t->Branch[2], out);
		out << ",";
		_serialize_to_JSON (t->Branch[3], out);
		out << "]";
		
	}

	// Inner node: discrete attribute
	if (t->NodeType == BrDiscr) {

		// Attribute
		out << "\"attribute\":\"" << AttName[t->Tested] << "\",";

		// Cut
		out << "\"cut\":0,";

		// Classification
		out << "\"classification\":0,";

		// Children
		out << "\"children\":[";
		for (int i = 2; i <= t->Forks; i++) {

			_serialize_to_JSON (t->Branch[i], out);
			out << (i != t->Forks ? "," : "");
			
		}
		out << "]";

	}
	
	// Leaf node
	else if (!t->NodeType) {

		// Attribute
		out << "\"attribute\":\"\",";

		// Cut
		out << "\"cut\":0,";

		// Classification
		out << "\"classification\":" << ClassName[t->Leaf] << ",";

		// Left
		out << "\"children\":null";

	}

	// Error
	else {
		assert (false);
	}
	
	// End of class
	out << "}";

}

char * serialize_to_JSON (Tree t) {

	std::stringstream out;
	_serialize_to_JSON (t, out);

	std::string str = out.str ();
	char * cstr = (char *) malloc ((str.length ()+1) * sizeof(char));
  	std::strcpy (cstr, str.c_str ());

	return cstr;

}


