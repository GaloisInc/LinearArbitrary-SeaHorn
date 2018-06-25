#ifndef _POLY_
#define _POLY_
#include "config.h"
#include "base_learner.h"
#include "svm.h"


class PolyLearner: public BaseLearner {
	public:
		PolyLearner(States* gsets, /*const char* solution_filename = NULL,*/ int (*func)(int*) = target_program, int max_iteration = Miter);

		~PolyLearner();

		virtual int save2file(const char*);

		virtual int learn();

		virtual std::string invariant(int n);

	protected:
		SVM* svm;
		int max_iteration;
};

#endif
