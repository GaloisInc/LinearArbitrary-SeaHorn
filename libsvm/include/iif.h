/** @file iif.h
 *  @brief Contains all the files that needed to be included by a new test.
 *  
 *  By include this file, it should resolve all the reference errors to the framework.
 *
 *  @author Li Jiaying
 *  @bug No found bugs
 */
#ifndef _IIF_H_
#define _IIF_H_

#include "config.h"
#include "instrumentation.h"
#include "ml_algo.h"
#include "svm.h"
//#include "svm_i.h"
#include "color.h"
#include "solution.h"
#include "polynomial.h"
#include "connector.h"
#include "classifier.h"
#include "states.h"
#include "base_learner.h"
#include "linear_learner.h"
#include "poly_learner.h"
//#include "rbf_learner.h"
#include "conjunctive_learner.h"
//#include "disjunctive_learner.h"
#include "iif_assert.h"

#include <iostream>
#include <float.h>
#include <string>
#include <cassert>
#include <iostream>
#include <fstream>

#include <cstdlib>
#include <signal.h>
#ifdef linux
#include <sys/time.h>
#include <unistd.h>
#endif


extern int minv, maxv;
extern std::string* variables;
extern int vnum;
extern VariablePowerArray* vparray;
extern int iifround;
extern int initseed;


namespace iif{
	class LearnerNode {
		public:
			LearnerNode();
			~LearnerNode();
			BaseLearner* learner;
			LearnerNode* next;
	};

	class iifContext {
		private:
			static void sig_alrm(int signo) {
				std::cout << "\nTIMEOUT!\n";
				exit(-1);
			}

		public:
			iifContext (States* ss);

			iifContext (const char* vfilename, int (*func)(int*), const char* func_name = "Unknown", 
					const char* pasttestcase = NULL, int timeout = 60*15);

			~iifContext();

			iifContext& addLearner(const char* learnerName);

			int learn(const char* solution_filename = NULL, const char* invfilename = "noname", int times = 1);

		private:
			States* gsets;
			LearnerNode* first;
			LearnerNode* last; 
			int timeout;
	};
}
#endif
