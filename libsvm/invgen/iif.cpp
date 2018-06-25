#include "iif.h"
using namespace iif;

int iifround = 0;
int initseed = 0;

LearnerNode::LearnerNode() {
	learner = NULL;
	next = NULL;
}
LearnerNode::~LearnerNode() {
	if (learner != NULL)
		delete learner;
}

iifContext::iifContext (States* ss) {
	gsets = ss;
	first = NULL;
	last = NULL;
	variables = NULL;
	vnum = 0;
}

iifContext::iifContext(const char* vfilename, int (*func)(int*), 
		const char* func_name, const char* dataset_fname, int timeout) {
	std::ifstream vfile(vfilename);
	vfile >> vnum;
	variables = new std::string[Cv0to4];
	vparray = new VariablePowerArray[Cv0to4];
	variables[0] = '1';
	for (int i = 0; i < Cv0to4; i++) {
		for(int j = 0; j < Nv; j++) {
			vparray[i][j] = 0;
		}
	}
	for (int i = 1; i <= Nv; i++) {
		vfile >> variables[i];
		vparray[i][i-1] = 1;
	}
	vfile.close();
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

	gsets = new States[3];
	gsets[NEGATIVE].label = NEGATIVE;
	gsets[POSITIVE].label = POSITIVE;
	gsets[QUESTION].label = QUESTION;
	//gsets[CNT_EMPL].label = CNT_EMPL;
	if (dataset_fname != NULL) {
		//std::cout << "dataset filename := " << dataset_fname << std::endl;
		std::ifstream fin(dataset_fname);
		if (fin) {
			int l, pn, nn;
			fin >> l >> pn >> nn;
			//std::cout << "l:= " << l << "  pn:=" << pn << "  nn:=" << nn << std::endl;
			gsets[POSITIVE].initFromFile(pn, fin);
			gsets[NEGATIVE].initFromFile(nn, fin);
			fin.close();
		}
	}
	first = NULL;
	last = NULL;
	register_program(func, func_name);
	this->timeout = timeout;
	//srand(time(NULL)); // initialize seed for rand() function
	srand(initseed); // initialize seed for rand() function
	std::cout << YELLOW << "INIT SEED: " << initseed << NORMAL << std::endl;
}

iifContext::~iifContext() {
	// clean up heap memory
	LearnerNode* p = first;
	LearnerNode* pp;

	while (p != NULL) {
		pp = p->next;
		delete p;
		p = pp;
	}
	delete []gsets;
	if (variables != NULL)
		delete []variables;
	if (vparray != NULL)
		delete []vparray;
}


iifContext& iifContext::addLearner(const char* learnerName) {
	BaseLearner* newLearner = NULL;
	if (strcmp(learnerName, "linear") == 0)
		newLearner = new LinearLearner(gsets);
	else if (strcmp(learnerName, "poly") == 0)
	   newLearner = new PolyLearner(gsets);
	else if (strcmp(learnerName, "conjunctive") == 0)
	   newLearner = new ConjunctiveLearner(gsets);
	/* 
	   else if (strcmp(learnerName, "poly") == 0)
	   newLearner = new PolyLearner(gsets);
	   else if (strcmp(learnerName, "rbf") == 0)
	   newLearner = new RbfLearner(gsets);
	   else if (strcmp(learnerName, "conjunctive") == 0)
	   newLearner = new ConjunctiveLearner(gsets);
	   else if (strcmp(learnerName, "disjunctive") == 0)
	   newLearner = new DisjunctiveLearner(gsets);
	   */

	if (last == NULL) {
		last = new LearnerNode();
		first = last;
	} else {
		last->next = new LearnerNode();
		last = last->next;
	}
	last->learner = newLearner;
	return *this;
}

int iifContext::learn(const char* last_cnt_fname, const char* invfilename, int times) {
#ifdef linux
	// we only support timeout in LINUX system
	// Because don't know how to easily implement the same function in windows system...:( 
	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
		exit(-1);
	alarm(timeout);
#endif
#ifdef __PRT_STATISTICS
#if 0
	std::ofstream of1("../tmp/statistics", std::ofstream::app);
	struct timeval tv;
	char tmbuf[64], buf[64];
	gettimeofday(&tv, NULL);
	//strftime(tmbuf, sizeof(tmbuf), "%Y-%m-%d %H:%M:%S", localtime(&tv.tv_sec));
	strftime(tmbuf, sizeof(tmbuf), "%H:%M:%S", localtime(&tv.tv_sec));
	snprintf(buf, sizeof(buf), "%s.%06ld", tmbuf, tv.tv_usec);
	of1 << buf << "\t\t";
	of1.close();
#endif
#endif

	LearnerNode* p = first;
	char filename[256]; 
	if (p && last_cnt_fname) 
#ifdef __PRT
		//std::cout << "Test on counter example ...\n";
#endif
		p->learner->runCounterExampleFile(last_cnt_fname);
#ifdef __PRT
		//std::cout << "Test on counter example DONE...\n";
#endif

	while (p) {
		if (p->learner->learn() == 0) {
#ifdef __DS_ENABLED
			//p->learner->save2file(filename);
			sprintf(filename, "%s.ds", (char*)invfilename);
			p->learner->save2file(filename);
#endif
			sprintf(filename, "%s.inv", (char*)invfilename);
			std::ofstream invFile(filename);
			invFile << p->learner->invariant(0);
			invFile.close();
			return 0;
		} else {
			p = p->next;
		}
	}
	return -1;
}
