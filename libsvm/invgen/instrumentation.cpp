#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include "instrumentation.h"
#include <assert.h>
#include "color.h"

bool _passP = false;
bool _passQ = false;
int assume_times = 0;
int assert_times = 0;
char lt[4][10] =  { "Negative", "Question", "Positive", "Bugtrace"};
char(*LabelTable)[10] = &lt[1];

double program_states[MstatesIn1trace * 2][Nv];
int state_index;

int addStateInt(int first ...)
{
	if (state_index >= 0.9 * MstatesIn1trace)
		if (rand() % (100 * state_index / MstatesIn1trace) > 1)
			return 0;
	if (state_index >= 0.999 * MstatesIn1trace)
		return 0;
	va_list ap;
	va_start(ap, first);
	program_states[state_index][0] = first;
	for (int i = 1; i < Nv; i++) {
		program_states[state_index][i] = va_arg(ap, int);
	}
	va_end(ap);

#ifdef __PRT_TRACE
	std::cout << BLUE << "(" << program_states[state_index][0];
	for (int i = 1; i < Nv; i++) {
		std::cout << "," << program_states[state_index][i];
	}
	std::cout << ")" << NORMAL;
#endif

	state_index++;
	if (state_index >= MstatesIn1trace) {
		std::cout << RED << "\nToo many states (>" << MstatesIn1trace << 
			") in one execution. Stop here.\n" << NORMAL;
		exit(-1);
	}
	return 0;
}

int addStateDouble(double first, ...)
{
	if (state_index >= 0.9 * MstatesIn1trace)
		if (rand() % (100 * state_index / MstatesIn1trace) > 1)
			return 0;
	if (state_index >= 0.999 * MstatesIn1trace)
		return 0;
	va_list ap;
	va_start(ap, first);
	program_states[state_index][0] = first;
	for (int i = 1; i < Nv; i++) {
		program_states[state_index][i] = va_arg(ap, double);
	}
	va_end(ap);

#ifdef __PRT_TRACE
	std::cout << BLUE << "(" << program_states[state_index][0];
	for (int i = 1; i < Nv; i++) {
		std::cout << "," << program_states[state_index][i];
	}
	std::cout << ")" << NORMAL;
#endif

	state_index++;
	if (state_index >= MstatesIn1trace) {
		std::cout << RED << "\nToo many states (>" << MstatesIn1trace << 
			") in one execution. Stop here.\n" << NORMAL;
		exit(-1);
	}
	return 0;
}


int beforeLoop()
{
	//std::cout << "---> before_loop";
	state_index = 0;
	_passP = false;
	_passQ = false;
	assume_times = 0;
	assert_times = 0;
	//std::cout << "[done]";
	return 0;
}


int afterLoop(States* gsets)
{
	int label = 0;
	assert(assume_times == 1);
	assert(assert_times == 1);
	if (_passP && _passQ) {
		label = POSITIVE;
	} else if (!_passP && !_passQ) {
		label = NEGATIVE; 
	} else if (!_passP && _passQ) {
#ifdef __QAS_POSITIVE
		//std::cout << "?->+ ";
		label = POSITIVE; 
#elif __QAS_NEGATIVE
		//std::cout << "?->- ";
		label = NEGATIVE;
#else
		//std::cout << "?->? ";
		label = QUESTION; 
#endif
	} else if (_passP && !_passQ) {
		label = CNT_EMPL;
		std::cout << RED << "\ncounter-example trace:  ";
		for (int i = 0; i < state_index; i++) {
			std::cout << "(" << program_states[i][0];
			for (int j = 1; j < Nv; j++)
				std::cout << "," << program_states[i][j];
			std::cout << ")->";
		}
		std::cout << "END[x]" << NORMAL << std::endl;
	}

#ifdef __PRT_TRACE
	std::cout << BLUE << "TRACE: ";
	for (int i = 0; i < state_index; i++) {
		std::cout << "(" << program_states[i][0];
		for (int j = 1; j < Nv; j++)
			std::cout << "," << program_states[i][j];
		std::cout << ")->";
	}
	std::cout << "END[" << label << "]" << NORMAL << std::endl;
#endif

	if (label == POSITIVE || label == NEGATIVE || label == QUESTION)
		gsets[label].addStates(program_states, state_index);
	return label;
}

void printRunResult(int rr) {
	switch (rr) {
		case NEGATIVE:
			std::cout << "-";
			return;
		case QUESTION:
			std::cout << "?";
			return;
		case POSITIVE:
			std::cout << "+";
			return;
		case CNT_EMPL:
			std::cout << "x";
			return;
	}
}

int mDouble(double* p)
{
	int a[Nv];
	for (int i = 0; i < Nv; i++)
		a[i] = static_cast<int>(p[i]);
	return mInt(a);
}

int mInt(int* p) { return target_program(p); }
