#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include <cstdlib>


extern int Nv, Cv0, Cv1, Cv2, Cv3, Cv4, Cv1to1, Cv1to2, Cv1to3, Cv1to4, Cv0to1, Cv0to2, Cv0to3, Cv0to4; 

#define _in_
#define _out_

#define _factor_polynomial_

/** @brief is a integer, which defines precision as pow(10, -PRECISION)  
 *		   This should be set in /CMakeLists.txt file
 *		   You'd better set this value in a scope [1, 12] 
 */
//#define PRECISION 1
#define PRECISION 2
//#define PRECISION 3

/** @brief The pointer to test program, DO NOT assign directly
 *		   Call register_program to set its value
 */
extern int (*target_program) (int*);

/** @brief defines the initial max number items contains by states set. 
 *		   Better to be a number larger than 1000 
 */
//int Mitems = 0;

/** @brief defines max number of states contains in one executionn. 
 *		   Better to be a number larger than 128 
 */
const int MstatesIn1trace = 1024;

/** @brief defines the number of tests runs initially. Should be a positive integer.
*/
//const int Nexe_init = 10 * Nv;
const int Nretry_init = 6;

/** @brief defines the number of tests runs after the first time. Should be a positive integer.
*/
//static int Nexe_after = 2 * Nv;

/** @brief defines the number of random tests runs each time, 
 *		   which is used to avoid bias caused by tests picking chioce. 
 *		   Should be a non-negative integer.
 */
//const int Nexe_rand = 0 * Nv;

/** @brief defines the max number of iterations tried by machine learning algorithm, 
 *		   Should be a positive integer. Usually set between 8-128
 */
const int Miter = 512;

const double density = 0.4;
const int base_step = 100;
//const int restricted_trainset_size = 2000*Nv;

// @brief converged_std defines the standard times for consecutive convergence before 
//		  the learnt classifier is regarded as candidate invariant
const int converged_std = 1;

bool register_program(int (*func)(int*), const char* func_name = 0);

extern std::string* variables;
class VariablePowerArray{
	private:
		int* _vtimes;
	public:
		VariablePowerArray () {
			_vtimes = new int[Nv];
		}

		~VariablePowerArray () {
			delete [] _vtimes;
		}
		
		int& operator[] (int i) {
			assert((i>=0) && (i<Nv));
			return _vtimes[i];
		}

		friend std::ostream& operator << (std::ostream& out, VariablePowerArray& vpa) {
			out << "[" << vpa[0];
			for (int i = 1; i < Nv; i++) {
				out << "," << vpa[i];
			}
			out <<"]";
			return out;
		}

		std::string toString() {
			std::ostringstream stm;
			stm << "1";
			for (int i = 0; i < Nv; i++) {
				if (_vtimes[i] == 0)
					continue;
				for (int j = 0; j < _vtimes[i]; j++) {
					stm << "*" << variables[i + 1]; 
				}
			}
			return stm.str();
		}
};

extern VariablePowerArray* vparray;
#endif
