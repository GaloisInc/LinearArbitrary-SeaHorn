/** @file classifier.h
 *
 *  @author Li Jiaying
 *  @bug no known bugs found.
 */
#ifndef _CLASSIFIER_
#define _CLASSIFIER_

#include "config.h"
#include "polynomial.h"
#include "connector.h"
#include "color.h"

#include <iostream>
#include <string.h>
#include <assert.h>

extern int Nv, Cv0, Cv1, Cv2, Cv3, Cv4, Cv1to1, Cv1to2, Cv1to3, Cv1to4, Cv0to1, Cv0to2, Cv0to3, Cv0to4; 


class Classifier{
	protected:
	public:
		Polynomial* polys;
		Connector* cts;

	public:
		int size;
		int max_size;
		Classifier(int maxsize = 64);
		~Classifier();

		int clear();
		Polynomial* operator[] (int index);
		Polynomial* operator[] (int index) const; 
		Classifier& operator+= (Polynomial& poly);
		Classifier& operator= (Polynomial& poly);
		Classifier& operator= (Classifier& rhs);

		int add(Polynomial& poly, Connector& ct);
		int add(Polynomial& poly, int type = CONJUNCT);

		bool factor(Polynomial& poly);

		//static int solver(const Classifier* cl, Solution& sol);

		bool roundoff();

		bool simplify(); 
		bool checkRedundancy(int l);
		// checkint whether the last polynomial can infer the others
		//bool resolveUniImplication();

		std::string toString() const;

		friend std::ostream& operator << (std::ostream& out, const Classifier& cs);
};


#endif
