/** @file solution.h
 *  @brief Defines the linear equation format and its solution format.
 *
 *  @author Li Jiaying
 *  @bug No known bugs. 
 */

#ifndef _SOLUTION_H_
#define _SOLUTION_H_

//#include "header.h"
#include "config.h"
//#include "z3++.h"
#include <cmath>
#include <cfloat>
#include <stdarg.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <string.h>
#include <assert.h>
#include "color.h"

extern int maxv;
extern int minv;

/** \class Solution
 *  @brief This class defines the format of a valid solution to an equation.
 *
 *  It contains values to each variants in an equation
 */
class Solution{
	public:
		/** @brief Default constructor.
		 *		   Set all its elements to value 0
		 */
		Solution() {
			val[0] = 0;
			for (int i = 1; i < Nv; i++) {
				val[i] = 0;
			}
		}

		/** @brief Most useful constructor
		 *		   Set its elements to the given values, order keeps
		 *		   
		 *	@param a0... each element values for a solution
		 */
		Solution(double a0, ...) {
			va_list ap;
			va_start(ap, a0);
			val[0] = a0;

			for (int i = 1; i < Nv; i++) {
				val[i] = va_arg(ap, double);
			}
			va_end(ap);
		}

		Solution(double* values) {
			for (int i = 0; i < Nv; i++) {
				val[i] = values[i];
			}
		}

		Solution(int* values) {
			for (int i = 0; i < Nv; i++) {
				val[i] = values[i];
			}
		}

		Solution& operator = (const double* values) {
			for (int i = 0; i < Nv; i++) {
				val[i] = values[i];
			}
			return *this;
		}

		Solution& operator = (const int* values) {
			for (int i = 0; i < Nv; i++) {
				val[i] = values[i];
			}
			return *this;
		}
		/** @brief support << operator
		 *		   simply output its elements as a tuple
		 *
		 *	@param sol The solution object to be output
		 */
		friend std::ostream& operator << (std::ostream& out, const Solution& sol) {
			out << "(" << sol.val[0];
			for (int j = 1; j < Nv; j++)
				out << "," << sol.val[j];
			out << ")";
			return out;
		}

		/** @brief support >> operator
		 */
		friend std::istream& operator >> (std::istream& in, Solution& sol) {
			for (int j = 0; j < Nv; j++)
				in >> sol.val[j];
			return in;
		}

		double& operator[](int index) {
			assert((index >= 0) && (index < Nv));
			return val[index];
		}

	//private:
		/// The data field of Solution, stores all the values as a solution to an Polynomial 
		double val[Nv];
};


#endif

