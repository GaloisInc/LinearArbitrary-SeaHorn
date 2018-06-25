#ifndef _SVM_I_H_
#define _SVM_I_H_

#include "color.h"
#include <iostream>
#include <map>

#include "Perceptron.hpp"
#include "lr.hpp"

extern int svm_c;
extern int svm_all;

class SVM_I : public MLalgo //SVM
{
	protected:
		svm_parameter param;
		svm_model* model;
		int poly_num;

		int max_size;

		MState* raw_mapped_data;
		MState* data; // [max_items * 2];
		double* label; // [max_items * 2];


		/*int resize(int new_size) {
			if (new_size <= max_size) return 0;
			assert (new_size >= max_size);
			int valid_size = problem.l;

			// enlarge max_size exponentially to cover all the data.
			while (new_size >= max_size) max_size *= 2;

			double** new_data = new double*[max_size];
			memmove(new_data, data, valid_size * sizeof(double**));
			delete []data;
			data = new_data;

			double* new_label = new double[max_size];
			memmove(new_label, label, valid_size * sizeof(double*));
			delete label;
			label = new_label;
			return 0;
		}*/

	public:
		svm_problem problem;
		MState* negative_mapped_data; 
		int positive_size;
		int negative_size;

#ifdef __TRAINSET_SIZE_RESTRICTED
		SVM_I(int type = 0, void (*f) (const char*) = NULL, int size = restricted_trainset_size+1) : max_size(2 * size) {
#else
		SVM_I(int type = 0, void (*f) (const char*) = NULL, int size = 1000000) : max_size(size) {
#endif
				prepare_svm_parameters(&param, type);
				if (f != NULL)
					svm_set_print_string_function(f);
				model = NULL;
				
				raw_mapped_data = new MState[max_size];
				for (int i = 0; i < max_size; i++) {
					raw_mapped_data[i] = new svm_node[Nv+1];
				}

				data = new MState[max_size];
				label = new double[max_size];
				for (int i = 0; i < max_size; i++)
					label[i] = -1;
				problem.l = 0;
				problem.x = (svm_node**)(data);
				problem.y = label;

#ifdef __DS_ENABLED
				problem.np = 0;
				problem.nn = 0;
#endif

				etimes = 0;
				poly_num = 0;
				negative_mapped_data = new MState[max_size];
				for (int i = 0; i < max_size; i++) {
					negative_mapped_data[i] = new svm_node[Nv+1];
				}
				negative_size = 0;
			}

		~SVM_I() {
			if (model != NULL) svm_free_and_destroy_model(&model);
#ifdef __PRT_DEBUG
			std::cout << "SVM_I deleted model\n";
#endif
			if (raw_mapped_data != NULL) {
				for (int i = 0; i < max_size; i++)	
					delete [] raw_mapped_data[i];
				delete []raw_mapped_data;
			}
#ifdef __PRT_DEBUG
			std::cout << "SVM_I deleted raw_mapped_data\n";
#endif
			if (negative_mapped_data != NULL) {
				for (int i = 0; i < max_size; i++)
					delete [] negative_mapped_data[i];
				delete []negative_mapped_data;
			}
#ifdef __PRT_DEBUG
			std::cout << "SVM_I deleted negative_mapped_data\n";
#endif
			if (data != NULL) {
				delete []data;
			}
#ifdef __PRT_DEBUG
			std::cout << "SVM_I deleted data\n";
#endif
			if (label != NULL) delete []label;
#ifdef __PRT_DEBUG
			std::cout << "SVM_I deleted label\n";
#endif
		}

		int makeTrainingSet(States* gsets, int& pre_psize, int& pre_nsize) {
			int cur_psize = gsets[POSITIVE].getSize();
			int cur_nsize = gsets[NEGATIVE].getSize();

#ifdef __PRT
			std::cout << GREEN << "++new" << NORMAL << "[" << cur_psize - pre_psize << "|"
				<< cur_nsize - pre_nsize  << "] ==> " << GREEN << "total" << NORMAL << "["
				<< cur_psize << "+|" << cur_nsize << "-]";
#endif

			int ret = cur_psize + cur_nsize - pre_psize - pre_nsize;

#ifdef __TRAINSET_SIZE_RESTRICTED
			int pstart = cur_psize > restricted_trainset_size ? cur_psize - restricted_trainset_size : 0;
			int plength = cur_psize - pstart;
	
			for (int i = 0; i < plength; i++) {
				mappingData(gsets[POSITIVE].values[pstart + i], raw_mapped_data[i], 1);
				data[i] = raw_mapped_data[i];
				label[i] = 1;
			}
			positive_size = plength;
			int nstart = cur_nsize > restricted_trainset_size ? cur_nsize - restricted_trainset_size : 0;
			int nlength = cur_nsize - nstart;
			for (int i = 0; i < nlength; i++) {
				mappingData(gsets[NEGATIVE].values[nstart + i], negative_mapped_data[i], 1);
			}
			negative_size = nlength;
			pre_psize = cur_psize;
			pre_nsize = cur_nsize;
			cur_psize = plength;
			cur_nsize = nlength;
#ifdef __PRT
			std::cout << RED << " RESTRICTED[" << cur_psize << "+|" << cur_nsize << "-]" << NORMAL;
#endif
			ret = plength + nlength;
#else
			// prepare new training data set
			// training set & label layout:
			// data :  0 | positive states ...
			// add new positive states at OFFSET: [pre_psize]

			int cur_index = pre_psize;
			for (int i = 0; i < cur_psize - pre_psize; i++) {
				mappingData(gsets[POSITIVE].values[pre_psize + i], raw_mapped_data[cur_index + i], 1);
				data[cur_index + i] = raw_mapped_data[cur_index + i];
				label[cur_index + i] = 1;
			}
			cur_index = pre_nsize;
			for (int i = 0; i < cur_nsize - pre_nsize; i++) {
				mappingData(gsets[NEGATIVE].values[pre_nsize + i], negative_mapped_data[cur_index + i], 1);
			}
			positive_size = cur_psize;
			negative_size = cur_nsize;
			pre_psize = cur_psize;
			pre_nsize = cur_nsize;
#endif

#ifdef __DS_ENABLED
			problem.np = cur_psize;
			problem.nn = cur_nsize;
#endif

			problem.l = cur_psize;
			return ret;
		}

		int learn (int choice) {
			if (problem.y == NULL || problem.x == NULL || negative_mapped_data == NULL) return -1;

			std::vector<svm_node*> positives;
			std::vector<svm_node*> negatives;

			std::vector<svm_node*> temp;

			for (int i = 0; i < problem.l; i++) {
				positives.push_back (raw_mapped_data[i]);
			} 
			for (int i = 0; i < negative_size; i++) {
				negatives.push_back (negative_mapped_data[i]);
			} 

			learn (positives, negatives, choice);

			return 0;
		}


		int learn (
			std::vector<svm_node*> positives, std::vector<svm_node*> negatives, int choice) {

//std::cout << "all pos = " << positives.size() << 
//							" all neg = " << negatives.size() << "\n";

			/*problem.l = 0;
			for (svm_node* positive : positives) {
				data[problem.l] = positive;
				label[problem.l] = 1;
				problem.l ++;
			}
			for (svm_node* negative : negatives) {
				data[problem.l] = negative;
				label[problem.l] = -1;
				problem.l ++;
			}


			std::cout << " NEW TRAINING SET:";
			for (int i = 0; i < problem.l; i++) {
				std::cout << "(" << problem.x[i][0].value;
				std::cout << ":" << problem.x[i][0].index;
				for (int j = 1; j < Nv; j++) {
					std::cout << "," << problem.x[i][j].value;
					std::cout << ":" << problem.x[i][j].index;
				}
				std::cout << ")";

				if (problem.y[i] == 1) std::cout << "+";
				if (problem.y[i] == -1) std::cout << "-";
				std::cout << "|";
			}
			std::cout << std::endl;


			setEtimes(1);
			param.C = 100000;
			model = svm_train(&problem, &param);
			Polynomial poly;
			svm_model_visualization(model, &poly);
			svm_free_and_destroy_model(&model);
			model = NULL; */

			Polynomial poly;
			learn (positives, negatives, &poly, choice);

			//poly.roundoff ();

			//std::cout << poly << "\n";

			std::vector<svm_node*> un_positives;
			std::vector<svm_node*> un_negatives;

			int pre_pos_size = -1; int pre_neg_size = -1;
			int fail = 0;

			while (true) {// Finding a meaningful classifier
				//std::cout << poly << "\n";
			
				un_positives.clear();
				un_negatives.clear();
				
				for (int i = 0; i < positives.size(); i++) {
					double res = Polynomial::calc(poly, positives.at(i));
					if (res < 0) {
						// Found a miss-classified positive
						un_positives.push_back (positives.at(i));
						positives.erase(positives.begin() + i);
						i--;
					}
				}
				std::vector<svm_node*> correct_negatives;
				for (int i = 0; i < negatives.size(); i++) {
					double res = Polynomial::calc(poly, negatives.at(i));
					if (res >= 0) {
						// Found a miss-classified negative
						un_negatives.push_back (negatives.at(i));
					} else {
						correct_negatives.push_back(negatives.at(i));
					}
				}

				if ((un_positives.size() > 0 && positives.size() == 0) || 
					(un_negatives.size() == negatives.size () && un_positives.size() == 0)) {
					// classifier does not change current positive samples and negatives samples.
					// i.e., it is not meaningful.

					if (positives.size () == 0) {
						for (svm_node* positive : un_positives) {
							positives.push_back (positive);
						}
					}
					
					if (positives.size () == pre_pos_size && negatives.size() == pre_neg_size) {
							// Fail as learned poly is not working ...
							fail = 1;
							break;	
					} else {
						pre_pos_size = positives.size();
						pre_neg_size = negatives.size();
					}

					std::map<int, int> candidates;
					std::vector<Polynomial> polys;
					std::vector<int> scores;

					//int m_score = eval (positives, negatives, &poly);

					if (positives.size () > negatives.size ()) {
						for (svm_node* positive : positives) {
							std::vector<svm_node*> one_positive;
							one_positive.push_back (positive);
							Polynomial one_poly;
							learn (one_positive, negatives, &one_poly, choice);
							polys.push_back (one_poly);
							int score = eval (positives, negatives, &one_poly);
							scores.push_back (score);

							one_poly.roundoff();

							int nvars = 0;
							for (int j = 1; j < one_poly.getDims(); j++) {
								if (one_poly.theta[j] != 0) nvars++; 
							}
							
							auto search = candidates.find(nvars);
							if (search != candidates.end()) {
								if (scores.at(search->second) < score) {
									search->second = scores.size()-1;
								}
							} else {
								candidates.insert(std::make_pair(nvars, scores.size()-1));
							}
							//if (score > maxScore) {
							//	poly = one_poly;
							//	maxScore = score;
							//}

							//if (score > m_score) {
							//	poly = one_poly;
							//	break;
							//}
						}

						int maxScore = -1;
						for(std::map<int, int>::iterator iter = candidates.begin();
								iter != candidates.end();
     								++iter )
						{
    							if (scores.at(iter->second) > maxScore) {
								poly = polys.at(iter->second);
								maxScore = scores.at(iter->second);
							}

							//polys.at(iter->second).roundoff();

							//std::cout << polys.at(iter->second) << " score: " << scores.at(iter->second) << "\n";
						}


						if (svm_all && maxScore > -1) {
							for(std::map<int, int>::iterator iter = candidates.begin();
								iter != candidates.end();
     								++iter )
							{
								double r = ((double)scores.at(iter->second)) / (double)maxScore;					
								if (r > 0.95 && r < 1) {
									// Other similar solutions are added
									if (cl.add(polys.at(iter->second), CONJUNCT) <= 0) {
										//std::cout << "Exceed the max number of polynomials.\n";
										return -1;
									}
								}
							}
						}
	 
						continue;
					} else {
						for (svm_node* negative : negatives) {
							std::vector<svm_node*> one_negative;
							one_negative.push_back (negative);
							Polynomial one_poly;
							learn (positives, one_negative, &one_poly, choice);
							polys.push_back (one_poly);
							int score = eval (positives, negatives, &one_poly);
							scores.push_back (score);

							one_poly.roundoff();
	
							int nvars = 0;	
							for (int j = 1; j < one_poly.getDims(); j++) {
								if (one_poly.theta[j] != 0) nvars++; 
							}
							auto search = candidates.find(nvars);
							if (search != candidates.end()) {
								if (scores.at(search->second) < score) {
									search->second = scores.size()-1;
								}
							} else {
								candidates.insert(std::make_pair(nvars, scores.size()-1));
							}

							//if (score > m_score) {
							//	poly = one_poly;
							//	break;
							//}
						} 


						int maxScore = -1;
						for(std::map<int, int>::iterator iter = candidates.begin();
								iter != candidates.end();
     								++iter )
						{
    							if (scores.at(iter->second) > maxScore) {
								poly = polys.at(iter->second);
								maxScore = scores.at(iter->second);
							}

							//polys.at(iter->second).roundoff();

							//std::cout << polys.at(iter->second) << " score: " << scores.at(iter->second) << "\n";
						}


						if (svm_all && maxScore > -1) {
							for(std::map<int, int>::iterator iter = candidates.begin();
								iter != candidates.end();
     								++iter )
							{
								double r = ((double)scores.at(iter->second)) / (double)maxScore;					
								if (r > 0.95 && r < 1) {
									// Other similar solutions are added
									if (cl.add(polys.at(iter->second), CONJUNCT) <= 0) {
										//std::cout << "Exceed the max number of polynomials.\n";
										return -1;
									}
								}
							}
						}

						continue;
					}
						//return 0;
				} else {
					//std::cout << "sep pos = " << positives.size() << 
					//		" sep neg = " << correct_negatives.size() << "\n";

					//std::cout << "un pos = " << un_positives.size() << 
					//		" un neg = " << un_negatives.size() << "\n";

					//std::cout << "pos = " << positives.size() << 
					//		" neg = " << negatives.size() << "\n";

					//learn (positives, correct_negatives, &poly);
					break; // Found what we want
				}
			} // break as found what we want

			if (!fail) {
				//poly.roundoff ();
				//std::cout << poly << "\n"; 

				if (cl.add(poly, CONJUNCT) <= 0) {
					//std::cout << "Exceed the max number of polynomials.\n";
					return -1;
				}

				// Learn a conjunction
				if (un_negatives.size() > 0) {
					learn (positives, un_negatives, choice);
				}	
			
				// Learn a disjunction
				if (un_positives.size() > 0) {
					learn (un_positives, negatives, choice);
				}

				return 0;
			} else {
				// As we got stuck on find a classifier that separate at least some examples.
				return -1;
			}
		}

		int learn (std::vector<svm_node*> positives, std::vector<svm_node*> negatives, Polynomial* poly, 
			int choice) 		{
			if (choice == 0) {
				return svm (positives, negatives,poly);
			} else if (choice == 1) {
				return perceptron (positives, negatives, poly);
			} else {
				return logregress (positives, negatives, poly);
			}
		} 

		int svm (std::vector<svm_node*> positives, std::vector<svm_node*> negatives, Polynomial* poly) {
			problem.l = 0;
			for (svm_node* positive : positives) {
				data[problem.l] = positive;
				label[problem.l] = 1;
				problem.l ++;
			}
			for (svm_node* negative : negatives) {
				data[problem.l] = negative;
				label[problem.l] = -1;
				problem.l ++;
			}

			/*std::cout << " NEW TRAINING SET:";
			for (int i = 0; i < problem.l; i++) {
				std::cout << "(" << problem.x[i][0].value;
				std::cout << ":" << problem.x[i][0].index;
				for (int j = 1; j < Nv; j++) {
					std::cout << "," << problem.x[i][j].value;
					std::cout << ":" << problem.x[i][j].index;
				}
				std::cout << ")";

				if (problem.y[i] == 1) std::cout << "+";
				if (problem.y[i] == -1) std::cout << "-";
				std::cout << "|";
			}
			std::cout << std::endl;*/

			setEtimes(1);
			param.C = svm_c; //10;
			model = svm_train(&problem, &param);
			//Polynomial poly;
			svm_model_visualization(model, poly); //&poly);
			svm_free_and_destroy_model(&model);

			model = NULL;
			return 0;
		}

		int perceptron (std::vector<svm_node*> positives, std::vector<svm_node*> negatives, Polynomial* poly) {
			problem.l = 0;
			for (svm_node* positive : positives) {
				data[problem.l] = positive;
				label[problem.l] = 1;
				problem.l ++;
			}
			for (svm_node* negative : negatives) {
				data[problem.l] = negative;
				label[problem.l] = -1;
				problem.l ++;
			}

			std::vector<TrainingItem> training_set;

			for (int i = 0; i < problem.l; i++) {
				std::vector<double> row;
				row.push_back (1.0);
				for (int j = 0; j < Nv; j++) {
					row.push_back (problem.x[i][j].value);
				}

				if (problem.y[i] == 1) {
					TrainingItem trainItem(true, row);
					training_set.push_back (trainItem);						
				}
				if (problem.y[i] == -1) {
					TrainingItem trainItem(false, row);
					training_set.push_back (trainItem);

				}				
			}
			
			Perceptron perceptron(Nv + 1);

			perceptron.train(training_set, 1000);
    
    			perceptron.display (poly);

			return 0;
		}

		int logregress (std::vector<svm_node*> positives, std::vector<svm_node*> negatives, Polynomial* poly) {
			problem.l = 0;
			for (svm_node* positive : positives) {
				data[problem.l] = positive;
				label[problem.l] = 1;
				problem.l ++;
			}
			for (svm_node* negative : negatives) {
				data[problem.l] = negative;
				label[problem.l] = -1;
				problem.l ++;
			}

			std::vector<std::vector<double> > poss;
			std::vector<std::vector<double> > negs;

			for (int i = 0; i < problem.l; i++) {
				std::vector<double> row;
				row.push_back (1.0);
				for (int j = 0; j < Nv; j++) {
					row.push_back (problem.x[i][j].value);
				}

				if (problem.y[i] == 1) {
					poss.push_back (row);						
				}
				if (problem.y[i] == -1) {
					negs.push_back (row);
				}				
			}

			lr_train (poss, negs, Nv+1, poly);

			return 0;
		} 

		int eval (std::vector<svm_node*> positives, std::vector<svm_node*> negatives, Polynomial* poly) {
			int score = 0;
			int pos_score = 0;
			int neg_score = 0;
			for (int i = 0; i < positives.size(); i++) {
				double res = Polynomial::calc(*poly, positives.at(i));
				if (res >= 0) {
					score ++; pos_score ++;
				}
			}
			for (int i = 0; i < negatives.size(); i++) {
				double res = Polynomial::calc(*poly, negatives.at(i));
				if (res < 0) {
					score ++; neg_score ++;			
				}			
			}

			Polynomial tmp;
			tmp = *poly;
			tmp.roundoff();

			//std::cout << tmp << ": +_score: " << pos_score << " -_score: " << neg_score << " total: " << score << "\n";

			return score;
		}

		void filter (int threshold) {
			// Remove hyperplanes with very large coefficients with user-supplied threshold.
			if (threshold != INT_MAX && threshold > 0) {
			//for (int i = 0; i < cl.size; i++) {
			//	cl.polys[i].agressiveRoundoff(threshold);
			//}
			for (int i = cl.size - 1; i >= 0; i--) {
				for (int j = 1; j < cl.polys[i].getDims(); j++) {
					if (cl.polys[i].theta[j] > threshold || cl.polys[i].theta[j] < -1 * threshold) {
						// Cut this hyperplane. 
						for (int k = i; k < cl.size - 1; k++)					
							cl.polys[k] = cl.polys[k+1];
						cl.size --;
						break;
					}
				}
			}
			}
			
			// Remove duplicated hyperplanes.
			for(int i = 0; i < cl.size; ++i)
        			for(int j = i+1; j < cl.size;)
        			{
            				if(cl.polys[i] == cl.polys[j])
            				{
                				for(int k = j; k < cl.size - 1; ++k)
                    					cl.polys[k] = cl.polys[k+1];
                    
                				--cl.size;
            				}
            				else
                				++j;
        			}
		}


		int convert_To_Attribute (std::ofstream& attr) {
			if (cl.size == 0) {
				attr << "true\n";
				return 0;
			}

			attr << std::setprecision(16);
			// Set up attribute (name, value) pairs
			for (int i = 0; i < cl.size; i++) {
				Polynomial poly;
				poly = cl.polys[i];
		
				attr << poly.theta[0];
				for (int j = 1; j < poly.getDims(); j++) {
					attr << " " << poly.theta[j];
				}
				if (i == cl.size - 1) {}
				else attr << "\n";
			}
			return 0;	
		}

		// Translate scalar samples to boolean samples
		// Generate all files to be consumed by C5
		int convert_To_C5 (std::string predName, std::ofstream& data, std::ofstream& attr) {
			// Set up a data file
			for (int i = 0; i < positive_size; i++) {
				data << predName << ",?";
				for (int j = 0; j < cl.size; j++) {
					double res = Polynomial::calc(*cl[j], raw_mapped_data[i]);
					//if (res >= 0) res = 1; else res = 0;
					data << "," << res;
				}
				data << ",true\n";
			}
			for (int i = 0; i < negative_size; i++) {
				data << predName << ",?";
				for (int j = 0; j < cl.size; j++) {
					double res = Polynomial::calc(*cl[j], negative_mapped_data[i]);
					//if (res >= 0) res = 1; else res = 0;
					data << "," << res;
				}
				if (i == negative_size - 1) {data << ",false";}
				else data << ",false\n";
			}

			convert_To_Attribute (attr);
			return 0;		
		}	


		int train() {
			if (problem.y == NULL || problem.x == NULL || negative_mapped_data == NULL) return -1;
			for (cl.size = 0; cl.size < cl.max_size;) {	
				// Guarantee learning and verifying agree on the same representation.
				cl.roundoff();	
				int misidx = -1;
				int ret = getMisclassified(misidx);
				if (ret == -1) return -1;  // something wrong in misclassified.
				if ((ret == 0) && (misidx == -1)) {	// can divide all the negative points correctly

#ifdef __PRT_SVM_I
					std::cout << GREEN << "finish classified..." << NORMAL << std::endl;
#endif				
					//cl.simplify();
					//cl.roundoff();
					return 0;
				}

#ifdef __PRT_SVM_I
				std::cout << "." << cl.size << ">"; // << std::endl;
#endif
				// there is some point which is misclassified by current dividers.

				if (stepTrain(misidx) < 0) {
					std::cout << "Can not classify state [index" << misidx << "](" << negative_mapped_data[misidx][0].value;
					for (int i = 1; i < Nv; i++) {
						std::cout << ", " << negative_mapped_data[misidx][i].value;
					}
					std::cout << ") against other " << problem.l << " positive states.\n";
					return -1;
				}				
			}
			std::cout << RED << "Can not divide all the data by SVM-I with"
				" equations number limit to " << cl.size  << "." << NORMAL << std::endl;
			//std::cerr << RED << "You need to increase the limit by modifying [classname::methodname]"
			//	"=SVM-I::SVM-I(..., int equ = **) " << NORMAL << std::endl;
			return -1;
		}

		double checkTrainingSet()
		{
			int total = problem.l + negative_size;
			int pass = 0;
			for (int i = 0; i < problem.l; i++) {
				/*
				   std::cout << problem.x[i][0];
				   for (int j = 1; j < Nv; j++)
				   std::cout << "," << problem.x[i][j];
				   std::cout << GREEN << "-1" << "->" << predict((double*)problem.x[i]) << NORMAL << " ";
				   */
				int presult = predict(problem.x[i]);
				if (presult == 0) {
					std::cout << "predict error in checkTrainingSet function.\n";
					return 0;
				}
				pass += (presult == 1) ? 1 : 0;
			}
			for (int i = 0; i < negative_size; i++) {
				/*
				   std::cout << negative_mapped_data[i][0];
				   for (int j = 1; j < Nv; j++)
				   std::cout << "," << negative_mapped_data[i][j];
				   std::cout << GREEN << "-1" << "->" << predict(negative_mapped_data[i]) << NORMAL << " ";
				   */
				//pass += (predict(negative_mapped_data[i]) < 0) ? 1 : 0;
				int presult = predict(negative_mapped_data[i]);
				if (presult == 0) {
					std::cout << "predict error in checkTrainingSet function.\n";
					return 0;
				}
				pass += (presult == -1) ? 1 : 0;
			}
			//std::cout << "<total=" << total << " pass=" << pass << ">" << std::endl;
			return (double)pass / total;
		}

		bool pointwiseSimplify()
		{
#ifdef __PRT_DEBUG
			std::cout << "point wise simplify classifiers...\n";
#endif
			for(int i = 0; i < cl.size; i++) {
#ifdef __PRT_DEBUG
				std::cout << "try [" << i << "] " << cl.polys[i] << "-->";
#endif
				if (cl.size <= 1) return true;
				if(partialCheckTrainingSet(i) == 1) {
					cl.polys[i--] = cl.polys[--cl.size];
#ifdef __PRT_DEBUG
					std::cout << RED << "removed\n" << NORMAL;
#endif
				}
#ifdef __PRT_DEBUG
				else {
					std::cout << GREEN << "keeped.\n" << NORMAL;
				}
#endif
			}
			return true;
		}

		double partialCheckTrainingSet(int removed_cl)
		{
			int total = problem.l + negative_size;
			int pass = 0;
			for (int i = 0; i < problem.l; i++) {
				int presult = partialPredict(problem.x[i], removed_cl);
				if (presult == 0) {
					std::cout << "predict error in partialCheckTrainingSet function.\n";
					return 0;
				}
				pass += (presult == 1) ? 1 : 0;
			}
			for (int i = 0; i < negative_size; i++) {
				int presult = partialPredict(negative_mapped_data[i], removed_cl);
				if (presult == 0) {
					std::cout << "predict error in partialCheckTrainingSet function.\n";
					return 0;
				}
				pass += (presult == -1) ? 1 : 0;
			}
			return (double)pass / total;
		}

		bool converged(Classifier& pre_cl) {
			if (pre_cl.size != cl.size) return false;

			std::vector<bool> similar_vector(cl.size, false);
			for (int i = 0; i < pre_cl.size; i++) {	
				// for all the equations in current state
				for (int j = 0; j < pre_cl.size; j++) {	
					// check all the equations in last state
					if ((similar_vector[j] == false) && (cl[i]->isSimilar(*pre_cl[j]) == true))  {	
						// the equation in last has not been set
						// and it is similar to the current equation 
#ifdef __PRT
						std::cout << GREEN <<  "<" << i << ":" << j << ">" << NORMAL;
#endif 
						similar_vector[j] = true;
						break;
					} 
#ifdef __PRT
					else {
						std::cout << RED << "<" << i << ":" << j << ">" << NORMAL;
					}
#endif 
				}
			}
			for (int i = 0; i < pre_cl.size; i++) {
				if (similar_vector[i] == false) {
					similar_vector.clear();
					return false;
				}
			}
			similar_vector.clear();
			return true;
		}


		friend std::ostream& operator << (std::ostream& out, const SVM_I& svm_i) {
			return svm_i._print(out);
		}

		std::ostream& _print(std::ostream& out) const {
			out << cl;
			return out;
			//out << std::setprecision(16);
			out << " \n\t ------------------------------------------------------";
			out << " \n\t |     " << cl[0]->toString();
			for (int i = 1; i < cl.size; i++) {
				out << " \n\t |  /\\ " << cl[i]->toString();
			}
			out << " \n\t ------------------------------------------------------";
			return out;
		}

		int getProblemSize() {
			return problem.l + negative_size;
		}


		int predict(MState v) {
			if (v == NULL) return -2;
			/*
			 * We use conjunction of positive as predictor.
			 * For example, (A >= 0) /\ (B >= 0) /\ (C >= 0) /\ ...
			 * Only when the give input pass all the equationss, it returns 1;
			 * Otherwise, -1 will be returned.
			 */
			double res = 0;
			if (res >= 0) {
				for (int i = 0; i < cl.size; i++) {
					res = Polynomial::calc(*cl[i], v);
					if (res < 0) {
						//for (int j = 0; j < Nv; j++) {
						//	std::cout << v[j] << " "; 
						//}
						//std::cout << "\n";
						//std::cout << "Rejected by " << *cl[i] << "\n";
						break;
					}
				}
			}

			return (res >= 0) ? 1 : -1;
		}

		int partialPredict(MState v, int removed_cl) {
			if (v == NULL) return 0;
			if (cl.size <= 1) return 0;
			if (removed_cl < 0 || removed_cl >= cl.size) return 0;
			/*
			 * We use conjunction of positive as predictor.
			 * For example, (A >= 0) /\ (B >= 0) /\ (C >= 0) /\ ...
			 * Only when the give input pass all the equationss, it returns 1;
			 * Otherwise, -1 will be returned.
			 */
			double res = 0;
			if (res >= 0) {
				for (int i = 0; i < cl.size; i++) {
					if (i == removed_cl)
						continue;
					res = Polynomial::calc(*cl[i], v);
					if (res < 0) break;
				}
			}

			return (res >= 0) ? 1 : -1;
		}

	private:
		double checkStepTrainingData() {
			int pass = 0;
			for (int i = 0; i < problem.l; i++) {
				pass += (predict((problem.x[i])) * problem.y[i] >= 0) ? 1 : 0;
#ifdef __PRT_SVM_I
				double predict_result = predict((double*)(problem.x[i]));
				if (predict_result * problem.y[i] < 0) {
					std::cout << RED << "Predict fault on: [" << problem.x[i][0];
					for (int j = 1; j < Nv; j++)
						std::cout << "," << problem.x[i][j];
					std::cout << "]" << predict_result << ":" << problem.y[i] << NORMAL;
				}
#endif
			}
#ifdef __PRT_SVM_I
			std::cout << "\t";
			std::cout << "\nCheck on training set result: " << pass << "/" << problem.l << "..";
#endif
			return (double)pass / problem.l;
		}

		int stepTrain(int negative_index) {
			if ((negative_index < 0) || (negative_index >= negative_size))
				return -1;

			label[problem.l] = -1;
			data[problem.l] = negative_mapped_data[negative_index];

			problem.l++;

#ifdef __PRT_SVM_I
			std::cout << " NEW TRAINING SET:";
			for (int i = 0; i < problem.l; i++) {
				std::cout << "(" << problem.x[i][0].value;
				std::cout << ":" << problem.x[i][0].index;
				for (int j = 1; j < Nv; j++) {
					std::cout << "," << problem.x[i][j].value;
					std::cout << ":" << problem.x[i][j].index;
				}
				std::cout << ")";

				if (problem.y[i] == 1) std::cout << "+";
				if (problem.y[i] == -1) std::cout << "-";
				std::cout << "|";
			}
			std::cout << std::endl;
#endif			

			double precision = 0;
			int et;
			for (et = 1; et <= 4; et++) {
				setEtimes(et);
				model = svm_train(&problem, &param);
				Polynomial poly;
				svm_model_visualization(model, &poly);

				if (cl.add(poly, CONJUNCT) <= 0) {
					std::cout << "Exceed the max number of polynomials.\n";
					return -1;
				}

				precision = checkStepTrainingData();
				
				svm_free_and_destroy_model(&model);

#ifdef __PRT_SVM_I
				std::cout << GREEN <<  poly << "\n" << NORMAL;
				std::cout << " precision=[" << precision * 100 << "%]." << std::endl;
				if (precision < 1) {
					std::cout << "[" << problem.x[problem.l - 1][0].value;
					for (int j = 1; j < Nv; j++)
						std::cout << "," << problem.x[problem.l - 1][j].value;
					std::cout << "] " << problem.y[problem.l - 1] << " --> ";
					std::cout << poly << " ";
					std::cout << " --> precision=[" << precision * 100 << "%]." << std::endl;
				}
#endif
				if (precision < 1) std::cout << "CAN NOT DIVIDE THE PICKED NEGATIVE FROM POSITIVES...\n";
				
				if (precision == 1)
					break;
				cl.size--;
			}

			problem.l--;

			if (et > 4) {
				std::cout << "et = " << et << "\n";
				return -1;
			}
			
			model = NULL;
			return 0;
		}

		// negative points may be misclassified.
		int getMisclassified(int& idx) {
			idx = 0;
			if (cl.size < 0) return -1;
			if (cl.size == 0) return 0;

			//int start = rand() % negative_size;
			int start = 0;
			for (int i = 0; i < negative_size; i++) {
				int k = (i + start) % negative_size;
				if (predict(negative_mapped_data[k]) >= 0) {
#ifdef __PRT_SVM_I
					std::cout << "\n [FAIL] @" << k << ": (" << negative_mapped_data[k][0];
					for (int j = 1; j < Nv; j++)
						std::cout << "," << negative_mapped_data[k][j];
					std::cout << ")  \t add it to training set... ==>" << std::endl;
#endif
					//std::cout << RED << "x@" << k << " " << NORMAL;
					idx = k;
					return 0;
				}
			}

			// there should be no negatives misclassified.
#ifdef __PRT_SVM_I
			std::cout << "\n [PASS] @all";
#endif
			idx = -1;
			//std::cout << RED << "x@" << -1 << " " << NORMAL;
			return 0;
		}
};

#endif /* _SVM_I_H */
