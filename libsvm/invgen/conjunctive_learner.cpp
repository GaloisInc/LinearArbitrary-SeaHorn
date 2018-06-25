#include "config.h"
#include "ml_algo.h"
#include "color.h"
#include "polynomial.h"
#include "conjunctive_learner.h"

#include <iostream>
#include <float.h>
#include <string.h>
#include <assert.h>

int Nv, Cv0, Cv1, Cv2, Cv3, Cv4, Cv1to1, Cv1to2, Cv1to3, Cv1to4, Cv0to1, Cv0to2, Cv0to3, Cv0to4; 

int pn;
int nn;
int Mitems;

char* predName;

int svm_c;   // Standard C parameter
int svm_all; // Can return more than one meaning full classifier.
int svm_threshold; // Avoid very large coefficient. 
int alg_choice = 0; // 0 (default) -- SVM; 1 -- Perceptron; 2 -- Logistic Regression 

static void print_null(const char *s) {}

ConjunctiveLearner::ConjunctiveLearner(States* gsets, int (*func)(int*), int max_iteration) 
	: BaseLearner(gsets, func) { 
		svm_i = new SVM_I(0, print_null, Mitems);
		this->max_iteration = max_iteration;
	}

ConjunctiveLearner::~ConjunctiveLearner() { 
	if (svm_i != NULL)
		delete svm_i;
}

int ConjunctiveLearner::learn()
{
	//std::cout << YELLOW << ">>>> Conjunctive Learner-----------------------\n" << NORMAL;  
	srand(time(NULL)); // initialize seed for rand() function
	int rnd;
	bool converged = false;
	Classifier pre_cl;
	int pre_psize = 0, pre_nsize = 0;

	double pass_rate = 1;
	for (rnd = 1; ((rnd <= max_iteration) && (pass_rate >= 1)); rnd++) {
		svm_i->makeTrainingSet(gsets, pre_psize, pre_nsize);
		int ret = svm_i->train();
		if (ret == -1) {
			std::cerr << RED << "[FAIL] ..... Can not divided by SVM_I." << std::endl << NORMAL;
			return -1;
		}
		//std::cout << "|-->> " << YELLOW << *svm_i << std::endl << NORMAL;
		
		pass_rate = svm_i->checkTrainingSet();

		if (pass_rate == 1) 
			std::cout << GREEN << " [" << pass_rate * 100 << "%]" << NORMAL;
		else 
			std::cout << RED << " [" << pass_rate * 100 << "%]" << NORMAL;

		if (pass_rate < 1) {
			std::cerr << RED << "[FAIL] ..... Can not divided by SVM_I." << std::endl << NORMAL;
			rnd++;
			break;	
		}

		std::cout << GREEN << " [PASS]" << std::endl << NORMAL;

		//svm_i->pointwiseSimplify();

		//svm_i->cl.clear();
		//break for now ... iterative learning ... 
		converged = true;
		break;
	}


	std::cout << "---------------------------------------------------\n";

	int ret = 0;
	if ((converged) && (rnd <= max_iteration)) {
		//svm_i->pointwiseSimplify();
		//svm_i->cl.roundoff();

		//svm_i->cl.simplify();
		std::cout << YELLOW << "  Hypothesis Invairant(Conjunctive): { ";
		std::cout << GREEN << svm_i->cl << YELLOW;
		std::cout << " }" << NORMAL << std::endl;
	} else {
		std::cout << "SVM learning can not get converged.\n";
	}

	if ((pass_rate < 1) || (rnd > max_iteration)) {
		std::cout << RED << "  Cannot divided by SVM_I perfectly.\n" << NORMAL;
		std::cout << pass_rate << "\t" << rnd << std::endl;
		ret = -1;
	}
	
	return ret;
}

void ConjunctiveLearner::saveInvariant (std::string inv_fname) {
	Classifier cl;
	cl = svm_i->cl;
	Connector* cts;
	cts = cl.cts;
	
	std::ofstream fout(inv_fname);

	for (int i = 0; i < cl.size; i++) {
		if (cts[i].getType () == DISJUNCT) {
			if (i == 0) {}
			else fout << "Disjunct\n";
		} else if (cts[i].getType () == CONJUNCT) {
			if (i == 0) {}
			else fout << "\n";			
		} else {
		}
		Polynomial poly;
		poly = cl.polys[i];
		
		fout << poly.theta[0];
		for (int j = 1; j < poly.getDims(); j++) {
			fout << " " << poly.theta[j];
		}
	}

	fout.close ();

	return;
}


int ConjunctiveLearner::save2file(const char* dsfilename) {
	//std::ofstream fout("../tmp/svm.ds");
#ifdef __DS_ENABLED
	std::ofstream fout(dsfilename);
	fout << svm_i->problem.np + svm_i->negative_size  << "\t" << svm_i->problem.np << "\t" << svm_i->negative_size << "\n";
	for (int i = 0; i < svm_i->problem.np; i++) {
		fout << 1;
		for (int j = 0; j < Nv; j++)
			fout << "\t" << j << ":" << (svm_i->problem.x[i][j]).value;
		fout << "\n";
	}
	for (int i = 0; i < svm_i->negative_size; i++) {
		fout << -1;
		for (int j = 0; j < Nv; j++)
			fout << "\t" << j << ":" << svm_i->negative_mapped_data[i][j];
		fout << "\n";
	}
	fout.close();
	//std::cout << "save to file succeed. ../tmp/svm.ds\n";
	std::cout << "save the training dataset to file " << dsfilename << "\n";
#endif
	return 0;
}


int ConjunctiveLearner::invariant(std::string m_C5filename) {
	int pre_psize = 0, pre_nsize = 0;

	svm_i->makeTrainingSet(gsets, pre_psize, pre_nsize);
	int ret = svm_i->learn(alg_choice);
	if (ret == -1) {
		std::cerr << RED << "[FAIL] ..... Can not divided by SVM_I." << std::endl << NORMAL;
		return -1;
	}

	for (int i = 0; i < svm_i->cl.size; i++) {
		svm_i->cl.polys[i].theta[0] = 0;
	}
	svm_i->cl.roundoff();
	svm_i->filter(svm_threshold);

	std::cout << YELLOW << "  Hypotheses: { ";
	//std::cout << GREEN << svm_i->cl << YELLOW;
	std::cout << " }" << NORMAL << std::endl;

	
	std::ofstream attr_of(m_C5filename + ".attr");
	svm_i-> convert_To_Attribute(attr_of);
	attr_of.close();

	return 0;
}

void ConjunctiveLearner::setupC5(std::string m_C5filename, std::string predName) {
	std::ofstream names_of(m_C5filename + ".names");
	if(!names_of)return;

	std::ofstream intervals_of(m_C5filename + ".intervals");
	if(!intervals_of)return;

	int lowerInterval = 2;
	int upperInterval = 2;

	names_of << "invariant.\n";

	//first attribute is the predicate names
	names_of << "$pc: ";

	names_of << "PRED_0,";
	names_of << predName << ".\n";

	std::string interval_line;

	names_of << "PRED_0!V_0" << ": continuous.\n";;
	upperInterval ++;
	interval_line = std::to_string(lowerInterval) + " " + std::to_string(upperInterval - 1) + "\n";
	intervals_of << interval_line;

	lowerInterval = upperInterval;
	upperInterval = lowerInterval;	

	for (int i = 0; i < svm_i->cl.size; i++) {
		names_of << predName << "!V_" << std::to_string(i) << ": continuous.\n";
		upperInterval ++;
	}

	interval_line = std::to_string(lowerInterval) + " " + std::to_string(upperInterval - 1) + "\n";
	intervals_of << interval_line;
			
      	names_of << "invariant: true, false.\n";
	names_of.close();
	intervals_of.close();
	
	std::ofstream data_of(m_C5filename + ".data");
	std::ofstream attr_of(m_C5filename + ".attr");
	svm_i->convert_To_C5(predName, data_of, attr_of);
	data_of.close();
	attr_of.close();
	

}

// svm -v number_of_vars —p positives -n negatives dataset_filename
void parse_command_line(int argc, char **argv, char *input_file_name)
{
	int i;
	
	// parse options
	for(i=1;i<argc;i++)
	{
		if(argv[i][0] != '-') {break;}
		if(++i>=argc) {
			std::cout << "Check input parameters for errors.\n";
			exit(1);
		}
		switch(argv[i-1][1])
		{
			case 'p':
				pn = atoi(argv[i]);
				break;
			case 'n':
				nn = atoi(argv[i]);
				break;
			case 'v':
				Nv = atoi(argv[i]);
				break;
			case 'f':
				predName = argv[i];
				break;
			case 'c':
				svm_c = atoi(argv[i]);
				break;
			case 'a':
				svm_all = atoi(argv[i]);
				break;
			case 't':
				svm_threshold = atoi(argv[i]);
				break;
			case 'g':
				alg_choice = atoi(argv[i]);
				break;
			default:
				fprintf(stderr,"Unknown option: -%c\n", argv[i-1][1]);
				exit(1);
		}
	}

	// determine filenames

	if(i>=argc) {
		std::cout << "A dataset file must be given.\n";
		exit(1);
	}

	strcpy(input_file_name, argv[i]);
}

void saveTrueOrFalse (bool b, std::string inv_fname) {
	std::ofstream fout(inv_fname);
	if (b) 
		fout << "true\n";
	else 
		fout << "false\n";
	fout.close ();
}

//svm-invgen -v 3 —p 6 -n 6 FromCmd.data 
int main(int argc, char **argv) {
	fprintf(stdout, "Call SVM For Invariant Learning.\n");

	char dataset_fname[1024];

	svm_c = 10;
	svm_all = 0;
	svm_threshold = INT_MAX;
	
	parse_command_line (argc, argv, dataset_fname);

	std::string dataset_fname_str (dataset_fname);
	std::string rawname = dataset_fname_str.substr(0, dataset_fname_str.find_first_of("."));
	std::string inv_fname = rawname + ".inv";

	if (pn == 0 && nn == 0) {
//		fprintf(stderr,"At least one sample must be provided.\n");
//		exit (1);
		saveTrueOrFalse (true, rawname + ".attr");
		fprintf(stdout, "SVM completes.\n");
		return 0;
	} else if (nn == 0) {
//		saveTrueOrFalse (true, inv_fname);
		saveTrueOrFalse (true, rawname + ".attr");
		fprintf(stdout, "SVM completes.\n");
		return 0;
	} else if (pn == 0) {
//		saveTrueOrFasle (false, inv_fname);
		saveTrueOrFalse (false, rawname + ".attr");
		fprintf(stdout, "SVM completes.\n");
		return 0;
	}

	Mitems = pn + nn;

	Cv0 = 1;
	Cv1 = Nv;
	Cv2 = (Nv * (Nv + 1) / (1 * 2));
	Cv3 = (Nv * (Nv + 1) * (Nv + 2) / (1 * 2 * 3));
	Cv4 = (Nv * (Nv + 1) * (Nv + 2) * (Nv + 3) / (1 * 2 * 3 * 4));

	Cv1to1 = (Cv1);
	Cv1to2 = (Cv1 + Cv2);
	Cv1to3 = (Cv1 + Cv2 + Cv3);
	Cv1to4 = (Cv1 + Cv2 + Cv3 + Cv4);

	Cv0to1 = (Cv0 + Cv1);
	Cv0to2 = (Cv0 + Cv1 + Cv2);
	Cv0to3 = (Cv0 + Cv1 + Cv2 + Cv3);
	Cv0to4 = (Cv0 + Cv1 + Cv2 + Cv3 + Cv4);

//	int N = 6;

	States* gsets = new States[2];
	gsets[POSITIVE].label = POSITIVE;
	gsets[NEGATIVE].label = NEGATIVE;
	std::ifstream fin(dataset_fname);
	if (fin) {
		gsets[POSITIVE].initFromFile(pn, fin);
		gsets[NEGATIVE].initFromFile(nn, fin);
		fin.close();
	} else {
		fprintf(stdout, "SVM fails.\n");
		return -1;
	}



/*	double** program_states = new double*[N];
	for (int i = 0; i < N; i++) {
		program_states[i] = new double[Nv];
	}
	program_states[0][0] = 1;
	program_states[0][1] = 1;
	program_states[0][2] = 2;

	program_states[1][0] = 2;
	program_states[1][1] = 7;
	program_states[1][2] = 9;

	program_states[2][0] = 6;
	program_states[2][1] = 4;
	program_states[2][2] = 10;

	program_states[3][0] = -6;
	program_states[3][1] = -4;
	program_states[3][2] = -10;

	program_states[4][0] = -6;
	program_states[4][1] = 2;
	program_states[4][2] = -4;

	program_states[5][0] = 26;
	program_states[5][1] = 39;
	program_states[5][2] = 65;

	gsets[POSITIVE].addStates(program_states, N);


	double** neg_program_states = new double*[N];
	for (int i = 0; i < N; i++) {
		neg_program_states[i] = new double[Nv];
	}
	neg_program_states[0][0] = 2;
	neg_program_states[0][1] = 5;
	neg_program_states[0][2] = 4;

	neg_program_states[1][0] = 9;
	neg_program_states[1][1] = 3;
	neg_program_states[1][2] = 5;

	neg_program_states[2][0] = 8;
	neg_program_states[2][1] = 7;
	neg_program_states[2][2] = 10;

	neg_program_states[3][0] = -8;
	neg_program_states[3][1] = -7;
	neg_program_states[3][2] = -10;

	neg_program_states[4][0] = -8;
	neg_program_states[4][1] = 4;
	neg_program_states[4][2] = 18;

	neg_program_states[5][0] = -8;
	neg_program_states[5][1] = 100;
	neg_program_states[5][2] = 93;
	
	gsets[NEGATIVE].addStates(neg_program_states, N);
*/


	ConjunctiveLearner learner(gsets, NULL, 1); 


	variables = new std::string[Cv0to4];
	vparray = new VariablePowerArray[Cv0to4];
	variables[0] = '1';
	for (int i = 0; i < Cv0to4; i++) {
		for(int j = 0; j < Nv; j++) {
			vparray[i][j] = 0;
		}
	}
	for (int i = 1; i <= Nv; i++) {
		variables[i] = "x" + std::to_string(i);
		vparray[i][i-1] = 1;
	}
	int index = Nv+1;
	for (int i = 0; i < Nv; i++) {
		for (int j = i; j < Nv; j++) {
			vparray[index][i]++;
			vparray[index][j]++;
			variables[index++] = /*"(" +*/ variables[i+1] + "*" + variables[j+1] /*+ ")"*/;
		}
	}
	for (int i = 0; i < Nv; i++) {
		for (int j = i; j < Nv; j++) {
			for (int k = j; k < Nv; k++) {
				vparray[index][i]++;
				vparray[index][j]++;
				vparray[index][k]++;
				variables[index++] = /*"(" +*/ variables[i+1] + "*" + variables[j+1] 
					+ "*" + variables[k+1] /*+ ")"*/;
			}
		}
	}
	for (int i = 0; i < Nv; i++) {
		for (int j = i; j < Nv; j++) {
			for (int k = j; k < Nv; k++) {
				for (int l = k; l < Nv; l++) {
					vparray[index][i]++;
					vparray[index][j]++;
					vparray[index][k]++;
					vparray[index][l]++;
					variables[index++] = /*"(" +*/ variables[i+1] + "*" + variables[j+1] 
						+ "*" + variables[k+1] + "*" + variables[l+1]/* + ")"*/;
				}
			}
		}
	}


	try
	{
    		//learner.learn();
		//learner.saveInvariant (inv_fname);


		learner.invariant (rawname);

		//std::string pred(predName);

		//learner.setupC5 (rawname, pred);
	}
	catch (std::exception& e)
	{
    		std::cout << "Exception catched : " << e.what() << std::endl;
	}

/*	if (program_states != NULL) {
		delete [] program_states;
	}
	
	if (neg_program_states != NULL) {
		delete [] neg_program_states;
	}
*/

	if (gsets != NULL)
		delete []gsets;
	if (variables != NULL)
		delete []variables;
	if (vparray != NULL)
		delete []vparray;
	
	return 0;
}
