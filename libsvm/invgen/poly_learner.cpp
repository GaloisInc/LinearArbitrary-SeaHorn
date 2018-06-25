#include "config.h"
#include "color.h"
#include "polynomial.h"
#include "classifier.h"
#include "poly_learner.h"

#include <iostream>
//#include <float.h>
//#include <string.h>
#include <assert.h>

static void print_null(const char *s) {}

PolyLearner::PolyLearner(States* gsets, int (*func)(int*), int max_iteration) 
	: BaseLearner(gsets, func) {
		svm = new SVM(0, print_null);
		this->max_iteration = max_iteration;
	}

PolyLearner::~PolyLearner() {
	if (svm != NULL)
		delete svm;
}

int PolyLearner::learn()
{
	std::cout << YELLOW << ">>>> Polynomial Learner-----------------------\n" << NORMAL;  
	int rnd;
	//bool similarLast = false;
	bool converged = false;
	int converged_time = 0;
	Classifier pre_cl;
	int pre_psize = 0, pre_nsize = 0;

	double pass_rate = 1;
	svm->setKernel(1);

	for (rnd = 1; ((rnd <= max_iteration) /*&& (pass_rate >= 1)*/); rnd++) {
		//if (rnd >= 2) srand(time(NULL));
		int zero_times = 0;

		int nexe = (rnd == 1) ? Nexe_init : Nexe_after;
#ifdef __PRT
		int step = 1;
		std::cout << RED << "[" << rnd << "]" << NORMAL;
		std::cout << RED << "Polynomail SVM------------------------{" << svm->etimes 
			<< "}------------------------------------------------------------------------------------\n\t(" 
			<< YELLOW << step++ << NORMAL << ") execute programs... [" << nexe + Nexe_rand << "] ";
#else
		std::cout << RED << "[" << rnd;
#endif

init_svm:
		//std::cout << std::endl << "\t-->selective sampling:\n\t";
		selectiveSampling(Nexe_rand, nexe, &pre_cl);
		//std::cout << "\t<--selective sampling:\n";

		if ((rnd == 1) && (gsets[POSITIVE].traces_num() == 0 || gsets[NEGATIVE].traces_num() == 0)) {
#ifdef __PRT
			if (gsets[POSITIVE].traces_num() == 0) 
				std::cout << RED << "\tZero Positive trace, execute program again.\n" << NORMAL;
			if (gsets[NEGATIVE].traces_num() == 0) 
				std::cout << RED << "\tZero Negative trace, execute program again.\n" << NORMAL;
#else
			std::cout << "+";
#endif

			if (++zero_times >= Nretry_init) {
				if (gsets[POSITIVE].traces_num() == 0) 
					std::cout << RED << "Can not get any positive trace. " << std::endl;
				else
					std::cout << RED << "Can not get any negative trace. " << std::endl;
				std::cout << " re-Run the system again OR modify your loop program.\n" << NORMAL;
				exit(-1);
			}
			if (maxv <= 100000) {maxv+=base_step;}
			if (minv >= -100000) {minv-=base_step;}
			goto init_svm;
		}

#ifdef __PRT
		std::cout << "\t(" << YELLOW << step++ << NORMAL << ") prepare training data... ";
#else
		std::cout << "]" << NORMAL;
#endif

#ifdef __PRT_STATISTICS
		std::cout << BLUE << "run" << NORMAL << "[#r" << random_samples << ",#s" << selective_samples << "] -->  ";
#endif
		if (svm->makeTrainingSet(gsets, pre_psize, pre_nsize) == 0) {
			if (++zero_times < Nretry_init)
				goto init_svm;
		}
		while (gsets[POSITIVE].getSize() + gsets[NEGATIVE].getSize() >= density * pow(maxv-minv, Nv)) {
			if (maxv <= 100000) {maxv+=base_step;}
			if (minv >= -100000) {minv-=base_step;}
		}

#ifdef __PRT
		std::cout << "\n\t(" << YELLOW << step++ << NORMAL << ") start training ...";
#endif
#ifdef __DS_ENABLED
		std::cout << "[" << svm->problem.np << "+:" << svm->problem.nn << "-]";
#endif

		if (svm->train() != 0) {
			//#ifdef __PRT
			std::cout << RED  << " [FAIL] \n Can not divided by polynomial SVM " << NORMAL << std::endl;
			//#endif
			return -1;
		}
		std::cout << "|-->> " << YELLOW << svm->cl << NORMAL << std::endl;
#ifdef __PRT
		std::cout << "\t(" << YELLOW << step++ << NORMAL << ") checking training traces.";
#endif
		pass_rate = svm->checkTrainingSet();

#ifdef __PRT
		if (pass_rate == 1) 
			std::cout << GREEN << " [" << pass_rate * 100 << "%]" << NORMAL;
		else 
			std::cout << RED << " [" << pass_rate * 100 << "%]" << NORMAL;
#endif

		if (pass_rate < 1) {
			std::cerr << RED << "[FAIL] ..... Can not divided by polynomial SVM." << std::endl << NORMAL;
			rnd++;
			break;	
		}
#ifdef __PRT
		std::cout << GREEN << " [PASS]" << std::endl << NORMAL;
#endif

#ifndef __SELECTIVE_SAMPLING_ENABLED
		converged = true;
		break;
#endif

#ifdef __QUESTION_TRACE_CHECK_ENABLED
#ifdef __PRT
		std::cout << "\t(" << YELLOW << step++ << NORMAL << ") check Question Traces:   ";
#endif
		if (svm->checkQuestionTraces(gsets[QUESTION]) != 0)
			continue;
#ifdef __PRT
		std::cout << "\n";
#endif
#endif

		/*
		 *	similarLast is used to store the convergence check return value for the last time.
		 *	We only admit convergence if the three consecutive round are converged.
		 *	This is to prevent in some round the points are too right to adjust the classifier.
		 */
		//svm->cl.roundoff();
#ifdef __PRT
		std::cout << "\t(" << YELLOW << step++ << NORMAL << ") check convergence:        ";
#endif

		if (svm->converged(pre_cl) == true) {
			converged_time++;
#ifdef __PRT
			std::cout << "[";
			for (int j = 0; j < converged_std - converged_time; j++)
				std::cout << "F";
			for (int j = 0; j < converged_time; j++)
				std::cout << "T";
			std::cout << "]  ";
#endif

			if (converged_time >= converged_std) {
#ifdef __PRT
				std::cout << "[SUCCESS] rounding off" << std::endl;
#endif
				converged = true;
				rnd++;
				break;
			}
		} else {
			converged_time = 0;
		}
#ifdef __PRT
		std::cout << "[FAIL] neXt round " << std::endl;
#endif

		pre_cl = svm->cl;
		svm->cl.clear();
	} // end of SVM training procedure

	std::cout << "--------------------------------------------------\n";

	int ret = 0;
	if ((converged) && (rnd <= max_iteration)) {
		Polynomial* poly = svm->cl[0];	
		svm->cl.clear();
		svm->cl.factor(*poly);
		svm->cl.roundoff();
		std::cout << YELLOW << "  Invariant Candidate(Polynomial): {  ";
		std::cout << GREEN << svm->cl.toString() << YELLOW;
		std::cout << "  }" << NORMAL << std::endl;
	}

	if ((pass_rate < 1) || (rnd >= max_iteration)) {
		std::cout << RED << "  Cannot divide by polynomial SVM perfectly.\n" << NORMAL;
		ret = -1;
	}

	return ret;
}

std::string PolyLearner::invariant(int n) {
	return svm->cl.toString();
}

int PolyLearner::save2file(const char* dsfilename) {
	printStatistics();
	//svm->problem.save_to_file("../tmp/svm.ds");
	//std::cout << "save to file succeed. ../tmp/svm.ds\n";
#ifdef __DS_ENABLED
	svm->problem.save_to_file(dsfilename);
	std::cout << "save the training dataset to file " << dsfilename << "\n";
#endif
	return 0;
}
