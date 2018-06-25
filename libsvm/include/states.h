#ifndef _STATES_H_
#define _STATES_H_
#include "config.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <string.h>

extern int Nv, Cv0, Cv1, Cv2, Cv3, Cv4, Cv1to1, Cv1to2, Cv1to3, Cv1to4, Cv0to1, Cv0to2, Cv0to3, Cv0to4, Mitems; 


//typedef double State[Nv];
typedef double* State;

class States{
	public:
		State (*values);
		int label;
		int size;

		// t_index is the array stored all the offset of traces in states.
		// e.g. t_index[0] = 0 means the 0-th trace is located at position 0 in values;
		// t_index[1] = 5 means the 1st trace is located at postion 5....
		//						and also the length of the 0-TH trace is 5!!!!!
		int* t_index;

		// p_index is the current position of the t_index.
		// Apperately, we have :
		//						t_index[p_index] == size
		// Thus, size is redundant in this context.
		int p_index;

		inline int getTraceSize() {
			return p_index;
		}

		inline int traces_num() {
			return p_index;
		}

		int getSize() {
			return size;
		}

		double* getState (int i) {
			if (i >= size) return NULL;
			return values[i];
		}

		int getLabel(int index = 0) { 
			return label; 
		}

	public:
		States() : max_size(Mitems) {
			values = new double*[Mitems];
			for (int i = 0; i < Mitems; i++) {
				values[i] = new double[Nv];
			}
			t_index = new int[Mitems];
			t_index[0] = 0;
			p_index = 0;
			size = 0;
		}

		~States();

		bool initFromFile(int num, std::ifstream& fin);

		int addStates(State st[], int len);

		void dumpTrace(int num);

		friend std::ostream& operator << (std::ostream& out, const States& ss);

	private:
		static bool stateCmp(const State& s1, const State& s2) {
			for (int i = 0; i < Nv; i++) {
				if (s1[i] != s2[i])
					return false;
			}
			return true;
		}
		static inline void stateCpy(State* dst, State* src, int length = 1) {
			memcpy(dst, src, sizeof(State) * length);
		}
		int max_size;
};

#endif
