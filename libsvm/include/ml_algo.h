/** @file ml_algo.h
 *  @brief Provide the base class for specific maching leanring algorithm.
 *
 *  This file contains all the necessary function support for specific machine learning algorithm.
 *
 *  @author Li Jiaying
 *  @bug 
 */
#ifndef _ML_ALGO_H_
#define _ML_ALGO_H_
#include <iostream>
#include "svm.h"
#include "states.h"
#include "polynomial.h"
#include "classifier.h"

typedef svm_node* MState;

enum {NEGATIVE = 0, POSITIVE, QUESTION, CNT_EMPL};

int DIMENSION = Nv;

class MLalgo 
{
	protected:
		int etimes;

	public:
		Classifier cl;
		MLalgo() {};
		virtual ~MLalgo() {};

		virtual int makeTrainingSet(States* gsets, int& pre_positive_size, int&pre_negative_size) = 0;
		virtual int train() = 0;
		virtual double checkTrainingSet() = 0;

		virtual bool converged(Classifier& pre_cl) = 0;

		friend std::ostream& operator << (std::ostream& out, const MLalgo& mla) { return mla._print(out); }

		virtual std::ostream& _print(std::ostream& out) const {
			out << "ML-algo: NULL";
			return out;
		};

		bool mappingData(double* src, svm_node* dst, int et = 4) {
			int index = 0;
			if (et >= 1) {
				for (int i = 0; i < Nv; i++) {
					dst[index].index = index+1;
					dst[index++].value = src[i];
				}
			}
			if (et >= 2) {
				for (int i = 0; i < Nv; i++) {
					for (int j = i; j < Nv; j++) {
						dst[index].index = index+1;
						dst[index++].value = src[i] * src[j];
					}
				}
			}
			if (et >= 3) {
				for (int i = 0; i < Nv; i++) {
					for (int j = i; j < Nv; j++) {
						for (int k = j; k < Nv; k++) {
							dst[index].index = index+1;
							dst[index++].value = src[i] * src[j] * src[k];
						}
					}
				}
			}
			if (et >= 4) {
				for (int i = 0; i < Nv; i++) {
					for (int j = i; j < Nv; j++) {
						for (int k = j; k < Nv; k++) {
							for (int l = k; l < Nv; l++) {
								dst[index].index = index+1;
								dst[index++].value = src[i] * src[j] * src[k] * src[l];
							}
						}
					}
				}
			}
			if (et >= 5) {
				std::cout << "Unsupported for 5 dimension up.\n";
				return false;
			}
			dst[index].index = -1;
			return true;
		}
		
		int setDimension(int d) {
			DIMENSION = d;
			return d;
		}

		bool setEtimes(int et) {
			if ((et < 1) || (et > 4))
				return false;
			etimes = et;
			switch (et) {
				case 1:
					return setDimension(Cv1to1);
				case 2:
					return setDimension(Cv1to2);
				case 3:
					return setDimension(Cv1to3);
				case 4:
					return setDimension(Cv1to4);
			}
			return true;
		}

		virtual int getProblemSize() = 0;

		virtual int predict(MState x) = 0;
	private:
};

static void print_string_stdout(const char *s)
{
	fputs(s,stdout);
	fflush(stdout);
}
static void (*svm_print_string) (const char *) = &print_string_stdout;
#if 1
static void info(const char *fmt,...)
{
	char buf[BUFSIZ];
	va_list ap;
	va_start(ap,fmt);
	vsprintf(buf,fmt,ap);
	va_end(ap);
	(*svm_print_string)(buf);
}
#else
static void info(const char *fmt,...) {}
#endif

void my_print_func(const char* str) {}


void prepare_svm_parameters(struct svm_parameter* param, int type = 0, int degree = 3) {
	param->svm_type = C_SVC;
	param->degree = degree;
	param->gamma = 0;	// 1/num_features
	if (type == 0) {
		std::cout << "Using LINEAR kernel...\n";
		param->kernel_type = LINEAR;
	} else if (type == 1){
		std::cout << "Using POLY kernel...\n";
		param->kernel_type = POLY;
		param->gamma = 8;//1.0/DIMENSION;	// 1/num_features
	} else if (type == 2){
		std::cout << "Using RBF kernel...\n";
		param->kernel_type = RBF;
		param->gamma = 20; ///DIMENSION; //0;	// 1/num_features
	}
	param->coef0 = 0;
	param->nu = 0.5;
	param->cache_size = 100;
	param->C = 100000000;
	//param->C = DBL_MAX;
	//param->C = 1000;
	param->eps = 1e-3;
	param->p = 0.1;
	param->shrinking = 1;
	param->probability = 0;
	param->nr_weight = 0;
	param->weight_label = NULL;
	param->weight = NULL;
	param->shrinking = 0;
	svm_set_print_string_function(my_print_func);
			}


int svm_model_visualization(const svm_model *model, Polynomial* poly = NULL)
{	
	if (model == NULL)
		return -1;
	if (poly == NULL)
		return -1;
	if (model->param.kernel_type != LINEAR) {
		info("Can not visualize hyperplane for non-linear kernel %s\n");
		return -1;
	}

	int l = model->l;
	const double * const *sv_coef = model->sv_coef;
	const svm_node * const *SV = model->SV;

	double theta[Cv0to4];// = poly->theta;
	for (int i = 0; i < Cv0to4; i++)
		theta[i] = 0;
	theta[0] = sv_coef[0][0] > 0? 1 : -1;

	for(int i=0;i<l;i++)
	{
		const svm_node *p0 = SV[0];
		//	double label[i] = sv_coef[0][i];
		double temp = 0;
		const svm_node *p = SV[i];
		for (int j = 0; j < DIMENSION; j++)
		{
			//double x[i][j] = p->value;
			theta[j+1] += sv_coef[0][i] * p->value;
			temp += p0->value * p->value; 
			p++;
			p0++;
		}
		temp *= sv_coef[0][i];
		theta[0] -= temp;
	}

	poly->setDims(DIMENSION + 1);

	poly->set(theta);

	//std::cout << BLUE << "Before RoundOFF " << *poly << GREEN << " +[-1, 1]" << NORMAL << std::endl;
	//poly->roundoff();
	//std::cout << BLUE << "from svm model visualization: " << *poly << std::endl << NORMAL;

	return 0;	
			}

#endif /* _ML_Algo_H */
