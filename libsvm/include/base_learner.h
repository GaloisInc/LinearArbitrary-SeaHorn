/** @file iif_learn.h
 *  @brief Provide base class for all iif learning algorithms.
 *
 *  This file contains the necessary function support for iif learning algorithms.
 *  The function run_target is the most important function in this base class for now. 
 *
 *  @author Li Jiaying
 *  @bug no known bugs found.
 */
#ifndef _LEARNER_BASE_
#define _LEARNER_BASE_

#include "config.h"
#include "states.h"
#include "polynomial.h"
#include "classifier.h"
//#include "candidates.h"
#include "instrumentation.h"
#include "color.h"
//#include "iif.h"

#include <iostream>
#include <iomanip>
#include <float.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <unistd.h>

extern int iifround;

extern int Nv, Cv0, Cv1, Cv2, Cv3, Cv4, Cv1to1, Cv1to2, Cv1to3, Cv1to4, Cv0to1, Cv0to2, Cv0to3, Cv0to4; 

class BaseLearner{
	public:
		BaseLearner(States* gsets, int (*func)(int*) = target_program):
			gsets(gsets), func(func) { }

		virtual ~BaseLearner() {
		} 

		virtual int save2file(const char*) = 0;
		
		virtual int learn() = 0;


	protected:
		States* gsets;
		int (*func)(int*);
};

#endif
