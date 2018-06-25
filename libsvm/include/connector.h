/** @file connector.h
 *
 *  @author Li Jiaying
 *  @bug no known bugs found.
 */
#ifndef _CONNECTOR_
#define _CONNECTOR_

#include "config.h"
#include "color.h"

#include <iostream>
#include <float.h>
#include <string.h>
#include <assert.h>

enum { UNUSED, DISJUNCT, CONJUNCT };

class Connector{
	public:
		Connector(int connector_type = 0) {
			if (connector_type >= 0)
				type = connector_type;
		}

		~Connector() { } 

		void setType(int connector_type) {
			type = connector_type;
		}

		int getType() {
			return type;
		}

		Connector& operator= (const Connector& rhs) {
			if (this == &rhs) return *this;
			type = rhs.type;
			return *this;
		}

	friend std::ostream& operator<< (std::ostream& out, const Connector& c) {
		switch (c.type) {
			case DISJUNCT:
				out << " || ";
				break;
			case CONJUNCT:
				out << " && ";
				break;
			default: // unknown connector
				out << " && false &&  ";
		}
		return out;
	}

	protected:
		int type;
};


#endif
