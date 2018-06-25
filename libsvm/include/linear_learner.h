#ifndef _LINEAR_
#define _LINEAR_
#include "config.h"
#include "base_learner.h"
#include "svm.h"


class LinearLearner: public BaseLearner {
	public:
		LinearLearner(States* gsets, /*const char* solution_filename = NULL,*/ int (*func)(int*) = target_program, int max_iteration = Miter);

		~LinearLearner();

		virtual int save2file(const char*);

		virtual int learn();

		virtual std::string invariant(int n);

	protected:
		SVM* svm;
		int max_iteration;
};

#endif
