#include "states.h"

States::~States() {
	if (values != NULL) {
		for (int i = 0; i < Mitems; i++)
			delete [] values[i];

		delete[] values;

		values = NULL;
	}
	if (t_index != NULL) {
		delete[] t_index;
		t_index = NULL;
	}
}

bool States::initFromFile(int num, std::ifstream& fin) {
	int label;
	for (int i = 0; i < num; i++) {
		fin >> label;
		for (int j = 0; j < Nv; j++) {
			fin >> values[i][j];
		}
	}
	size += num;
	t_index[p_index + 1] = t_index[p_index] + num;
	p_index++;
	return true;
}

int States::addStates(State st[], int len) {
	if (size + len >= max_size) {
		//std::cerr << "exceed maximium program states." << std::endl;
		int previous_max_size = max_size;
		double ** previous_values = values;
		//while (t_index[p_index] + len >= max_size)
		max_size *= 2;

		values = new double*[max_size];
		if ((values) == NULL)
			return -1;
		for (int i = 0; i < max_size; i++) {
			values[i] = new double[Nv];
		}
		memcpy(values, previous_values, previous_max_size * Nv * sizeof(double));


		int* previous_t_index = t_index;
		if ((t_index = new int[max_size]) == NULL)
			return -1;
		memcpy(t_index, previous_t_index, previous_max_size * sizeof(int));
	}

	int addLength = 0;
	for (int i = 0; i < len; i++) {
		// try to insert state st[i]
		bool skip = false;
		for (int j = 0; j < size; j++) {
			if (stateCmp(values[j], st[i]) == true) {
				skip = true;
				break;
			}
		}
		if (skip) continue;
		stateCpy(&values[size], &st[i]);
		addLength++;
		size++;
	}
	t_index[p_index + 1] = t_index[p_index] + addLength;
	p_index++;
	//std::cout << "+" << addLength << " ";
	return addLength;
}

void States::dumpTrace(int num) {
	if (num >= p_index) {
		std::cerr << "exceed state set boundary" << std::endl;
		return;
	}
	for (int i = t_index[num]; i < t_index[num + 1]; i++) {
		std::cout << "(" << values[i][0];
		for (int j = 1; j < Nv; j++)
			std::cout << "," << values[i][j];
		std::cout << ")->";
	}
	std::cout << "end.";
}

std::ostream& operator<< (std::ostream& out, const States& ss) {
	//std::cout << "lable[" << ss.label << "]:" << std::endl;
	for (int i = 0; i < ss.p_index; i++) {
		std::cout << "\tTr." << i << ":";
		for (int j = ss.t_index[i]; j < ss.t_index[i + 1]; j++) {
			std::cout << "(" << ss.values[j][0];
			for (int k = 1; k < Nv; k++)
				std::cout << "," << ss.values[j][k];
			std::cout << ")->";
		}
		std::cout << "end." << std::endl;
	}
	return out;
}
