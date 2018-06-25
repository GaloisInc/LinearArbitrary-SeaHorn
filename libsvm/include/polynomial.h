/** @file polynomail.h
 *  @brief Defines the linear polynomail format and its solution format.
 *
 *  @author Li Jiaying
 *  @bug No known bugs.
 */

#ifndef _POLYNOMIAL_H_
#define _POLYNOMIAL_H_

#include "config.h"
#include <cmath>
#include <cfloat>
#include <stdarg.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>

#include "color.h"
#include "svm.h"


extern int maxv;
extern int minv;
extern std::string* variables;
extern VariablePowerArray* vparray;
extern int vnum;

extern int Nv, Cv0, Cv1, Cv2, Cv3, Cv4, Cv1to1, Cv1to2, Cv1to3, Cv1to4, Cv0to1, Cv0to2, Cv0to3, Cv0to4; 


/** \class Polynomial
 *  @brief This class defines an polynomail by storing all its coefficiencies.
 *		   An polynomail is regarded a hyperplane in math.
 *
 *  theta[0] * 1 + theta[1] * x_1 + theta[1] * x_2 + ... + theta[Cv1to4] * x_{Cv0to4} >= 0
 */
class Polynomial {
	private:
		int dims;
		int etimes;

	public:
		/** @brief Default constructor.
		 *		   Set all its elements to value 0
		 */
		Polynomial() {
			theta = new double[Cv0to4];
			setEtimes(1);
			for (int i = 1; i < Cv0to4; i++) {
				theta[i] = 0;
			}
		}

		
		~Polynomial() {
			delete [] theta;
		}

		bool set(double* values) {
			for (int i = 0; i < dims; i++) {
				theta[i] = values[i];
			}
			return true;
		}

		bool set(int* values) {
			for (int i = 0; i < dims; i++) {
				theta[i] = values[i];
			}
			return true;
		}

		/** @brief Most useful constructor
		 *		   Set its elements to the given values, order keeps
		 *		   The first element is Theta0
		 */
		Polynomial(double a0, ...) {
			setEtimes(1);
			va_list ap;
			va_start(ap, a0);
			theta[0] = a0;

			for (int i = 1; i < dims; i++) {
				theta[i] = va_arg(ap, double);
			}
			va_end(ap);
		}

		/** @brief Copy constructor.
		 *
		 * @param poly The polynomail to be copied.
		 */
		Polynomial(const Polynomial& poly) {			
			theta = new double[Cv0to4];
			setEtimes(poly.getEtimes());
			for (int i = 0; i < poly.getDims(); i++) {
				theta[i] = poly.theta[i];
			}
		}

		/** @brief Overwrite = operator
		 *
		 *	This is needed when we want to delete a polynomail in an polynomail list
		 *	We copy the next polynomail to the current one, and repeat this process until tails
		 *
		 *	@param rhs The right-hand-side polynomail of assignment
		 */
		Polynomial& operator=(Polynomial& rhs);
		bool operator==(const Polynomial& rhs);

		std::string toString() const;

		/** @brief Output the polynomail in a readable format
		 *
		 *	Example:
		 *  5 + 2{0} + 3{1} >= 0
		 *
		 *  @param poly the polynomail to be ouput
		 */
		friend std::ostream& operator<< (std::ostream& out, const Polynomial& poly);

		/** @brief This uniImply method checks whether this polynomail object can uniImply another one or not
		 *		   That is to say:  *this ==> e2 ??
		 *		   *this is default polynomail left side
		 *
		 *  Currently, it is based on Z3 prover.
		 *  And the default precision is set to E-8 (2.8f), which is changeable if need
		 *
		 *  @param e2 is the polynomail right side
		 *  @return bool true if yes, false if no.
		 */
		bool uniImply(const Polynomial& e2);
		static bool multiImply(const Polynomial* e1, int e1_num, const Polynomial& e2);

		double evaluateItem(int index, const double* given_values) {
			assert ((index >= 0) && (index < dims));
			double result = theta[index];
			for (int i = 0; (i < Nv) && (result != 0); i++) {
				if (vparray[index][i] != 0) {
					result *= pow(given_values[i], vparray[index][i]);
				}
			}
			return result;
		}

		double evaluateCoef(int x, int power, double* given_values) {
			double result = 0;
			given_values[x] = 1;
			for (int i = 0; i < dims; i++) {
				if (vparray[i][x] == power)
					result += evaluateItem(i, given_values);
			}
			return result;
		}

		bool factor() {
			return true;
		}

		/** @brief This static method is used to get the position info for the given point against given polynomail
		 *
		 *	It just substitutes variants with the given point.
		 *
		 *	@param poly is the given polynomail, should not be NULL
		 *	@param sol is the tested solution, should not be NULL
		 *	@return The distance/value of the solution to the given polynomail
		 */
		static double calc(Polynomial& poly, svm_node* sol) {
			if (sol == NULL) return -1;
			//if (&poly == NULL) return -1;
			double res = 0;
			for (int i = 0; i < poly.getDims(); i++) {
				double tmp = poly.theta[i];
				for (int j = 0; j < Nv; j++) {
					int power = vparray[i][j];
					while (power-->0)
						tmp *= sol[j].value;
				}
				res += tmp;
			}
			return res;
		}

		/** @brief This method is used to check whether *this polynomail is similar to given polynomail e or not.
		 *		   *this ~= e ???
		 *
		 * @param precision defines how much variance we can bare.
		 *		  The default is 4, which means we can bare 0.0001 difference.
		 *		  In this case 1 ~=1.00001, but 1!~=1.000011
		 */
		bool isSimilar(Polynomial& e2, int precision = PRECISION) {
			//precision += 6;
			if (dims != e2.getDims()) return -1;
			double ratio = 0;
			int i;
			for (i = 0; i < dims; i++) {
				if ((theta[i] != 0) && (e2.theta[i] != 0)) {
					ratio = theta[i] / e2.theta[i];
					break;
				}
			}
			if (i >= dims)
				return -1;
			//std::cout << "1[ratio=" << ratio <<"]\n";
			double down, up;
			if (ratio >= 0) {
				down = ratio * (1 - pow(0.1, precision));
				up = ratio * (1 + pow(0.1, precision));
			}
			else {
				up = ratio * (1 - pow(0.1, precision));
				down = ratio * (1 + pow(0.1, precision));
			}
			//std::cout << "[" << down << ", " << ratio << ", " << up << "]";
			for (int i = 0; i < dims; i++) {
				if (e2.theta[i] >= 0) {
					if ((theta[i] < e2.theta[i] * down) || (theta[i] > e2.theta[i] * up))
						return false;
				}
				else {
					if ((theta[i] < e2.theta[i] * up) || (theta[i] > e2.theta[i] * down))
						return false;
				}
			}
			return true;
		}

		/** @brief Do roundoff job for an polynomail
		 *
		 *	Sometimes the polynomail has ugly coefficiencies
		 *	we want to make it elegent, which is the purpose of involing this method
		 *	Currently we have not done much work on this
		 *	We have not even use gcd function to adjust the coefficients.
		 *
		 *	For example.
		 *	1.2345 x1 >= 2.4690    ==>		x1 >= 2
		 *	2 x1 >= 5.000001	   ==>		x1 >= 2.5
		 *
		 *	@param e Contains the polynomail that has already rounded off
		 *	@return int 0 if no error.
		 */
		int roundoff(Polynomial& e);
		bool alreadyRoundoff();
		int agressiveRoundoff(int threshold);
		Polynomial* roundoff();

		//int toCandidates(Candidates* cs);

		inline double getTheta(int i) const {
			assert((i < dims) || "parameter for getTheta is out of boundary.");
			return theta[i];
		}
		inline bool setTheta(int i, double value) {
			assert((i < dims) || "parameter for getTheta is out of boundary.");
			theta[i] = value;
			return true;
		}
		inline int setTheta(double* values) {
			for (int i = 1; i < dims; i++)
				theta[i] = values[i];
			return true;
		}
		inline double getTheta0() const { return theta[0]; }
		inline bool setTheta0(double value) { theta[0] = value; return true; }

		inline int getEtimes() const {
			return etimes;
		}

		bool setEtimes(int et) {
			etimes = et;
			if (et == 1) { dims = Cv0to1; return true; }
			if (et == 2) { dims = Cv0to2; return true; }
			if (et == 3) { dims = Cv0to3; return true; }
			if (et == 4) { dims = Cv0to4; return true; }
			return false;
		}

		bool setDims(int dim) {
			if (dim == Cv0to1) { dims = dim; etimes = 1; return true; }
			if (dim == Cv0to2) { dims = dim; etimes = 2; return true; }
			if (dim == Cv0to3) { dims = dim; etimes = 3; return true; }
			if (dim == Cv0to4) { dims = dim; etimes = 4; return true; }
			return false;
		}

		inline int getDims() const {
			return dims;
		}

		double& operator[](int index) {
			assert (index < dims); 
			return theta[index];
		}
		//protected:
	public:
		double* theta;
};

#endif

