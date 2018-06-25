#include "config.h"
#include <iostream>
#include <stdlib.h>

extern int assume_times, assert_times;
int(*target_program)(int*) = NULL;

int minv = -1 * base_step, maxv = base_step;
std::string* variables;
VariablePowerArray* vparray;
int vnum;
#ifdef __PRT_STATISTICS
int random_samples = 0, selective_samples = 0;
#endif

bool check_target_program(int (*func)(int*))
{
	return true;
}


bool register_program(int (*func)(int*), const char* func_name) 
{
	if (check_target_program(func) == false) {
		if (func_name == NULL) {
			std::cout << "The target is not a valid program to test.\n";
		} else {
			std::cout << func_name << " is not a valid program to test.\n";
		}
		return false;
	}
	target_program = func;
	return true;
}
