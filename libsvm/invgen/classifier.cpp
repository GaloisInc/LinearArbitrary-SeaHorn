/** @file classifier.h
 *
 *  @author Li Jiaying
 *  @bug no known bugs found.
 */
#include "classifier.h"

Classifier:: Classifier(int maxsize) {
	max_size = maxsize;
	polys = new Polynomial[max_size];
	cts = new Connector[max_size];
	size = 0;
}

Classifier::~Classifier() { 
	if (polys) delete []polys;
	if (cts) delete []cts;
} 

int Classifier::clear() {
	size = 0;
	return 0;
}

Polynomial* Classifier::operator[] (int index) {
	if ((index >=0) && (index < size))
		return &polys[index];
	else
		return NULL;
}

Polynomial* Classifier::operator[] (int index) const {
	if ((index >=0) && (index < size))
		return &polys[index];
	else
		return NULL;
}

/*Classifier& Classifier::operator--(int n) {
  if (size >= 1)
  size -= 1;
  else
  size = 0;
  return *this;
  }
  */

Classifier& Classifier::operator+= (Polynomial& poly) {
	int res = add(poly, CONJUNCT);
	//assert(res >= 1);
	if (res <= 0)
		std::cout << "Bug here?\n";
	return *this;
}

Classifier& Classifier::operator= (Polynomial& poly) {
	clear();
	int res = add(poly, CONJUNCT);
	assert(res >= 1);
	return *this;
}

Classifier& Classifier::operator= (Classifier& rhs) {
	size = rhs.size;
	for (int i = 0; i < rhs.size; i++) {
		polys[i] = *rhs[i];
		cts[i] = rhs.cts[i];
	}
	return *this;
}

int Classifier::add(Polynomial& poly, Connector& ct) {
	if (size >= max_size)
		return -1;
	//poly.roundoff();
	polys[size] = poly;
	cts[size] = ct;
	return ++size;
}

int Classifier::add(Polynomial& poly, int type) {
	if (size >= max_size)
		return -1;
	//poly.roundoff();
	polys[size] = poly;
	cts[size].setType(type);
	return ++size;
}

bool Classifier::simplify() {
	if (size <= 1) return true;
#ifdef __PRT_INFER
	std::cout << YELLOW << "Simplify classifier..." << NORMAL << *this << "\n";
#endif
	for (int i = 0; (i < size) && (size >= 2); i++) {
#ifdef __PRT_INFER
		std::cout << BLUE << "Checking" << NORMAL << " all the others => " << polys[i] << " ";
#endif
		if (checkRedundancy(i) == true) {
#ifdef __PRT_INFER
			std::cout << GREEN << "TRUE\n" << NORMAL;
#endif
#if 1
			//polys[i--] = polys[--size];
			polys[i] = polys[size-1];
			i--;
			size--;
#else
			size--;
			for (int j = i; j < size; j++)
				polys[j] = polys[j+1];
			i--;
#endif
		} else {
#ifdef __PRT_INFER
			std::cout << RED << "FALSE\n" << NORMAL;
#endif
		}
	}

#ifdef __PRT_INFER
	std::cout << "after simplification..." << *this << "\n";
#endif
	return true;
}

bool Classifier::checkRedundancy(int l) {
	if (l < 0 || l >= size) return false; 

	return false;
}

bool Classifier::factor(Polynomial& poly) {
	//std::cout << "\tFactorization Process >>>>\n";
	//std::cout << GREEN << poly << NORMAL <<std::endl;
	int etimes = poly.getEtimes();
	if (etimes == 1) {
		//std::cout << "univariant linear function: " << poly << "\n";
		this->add(poly);
		return true;
	}

	if (Nv == 1) {
		this->add(poly);
		return true;

		if (etimes == 2) {
			// univariant quadratic function
			//std::cout << "univariant quadratic function: " << poly << "\n";
			double A, B, C;
			A = poly.getTheta(2);
			B = poly.getTheta(1);
			C = poly.getTheta(0);
			if (A == 0) {
				poly.setEtimes(1);
				this->add(poly);
				return true;
			}
			double delta = B * B - 4 * A * C;
			//std::cout << "A=" << A << " B=" << B << " C= " << C << " delta=" << delta << std::endl;
			if (delta < 0) {
				std::cout << RED << "Delta shouldnot be less than 0.\n";
				return false;
			}
			double x1, x2;
			x1 = (-1 * B + sqrt(delta)) / (2 * A);
			x2 = (-1 * B - sqrt(delta)) / (2 * A);
			if (x1 > x2) std::swap(x1, x2);
			//std::cout << "x1=" << x1 << ", x2=" << x2 << "\n";
			if (A > 0) {
				this->add(*(new Polynomial(floor(x1), -1.0)));
				this->add(*(new Polynomial(-1 * ceil(x2), 1.0)), DISJUNCT);
			} else {
				this->add(*(new Polynomial(-1 * ceil(x1), 1.0)));
				this->add(*(new Polynomial(floor(x2), -1.0)));
			}
			//std::cout << *this << std::endl;
			return true;
		}
	} else {
		this->add(poly);
		return true;
	}
	return false;
}

bool Classifier::roundoff() {
	for (int i = 0; i < size; i++) 
		polys[i].roundoff();
	return true;
}

#if 0
// checkint whether the last polynomial can infer the others
bool Classifier::resolveUniImplication() {
	if (size <= 1) return false;
#ifdef __PRT_SVM_I
	std::cout << "Resolving implication in Classifier, size = " << size;
#endif
	for (int i = 0; i < size - 1; i++) {
		//std::cout << RED << "\t??" << polys[size - 1] << " ==> " << polys[i] << "??";
		if (polys[size - 1].uniImply(polys[i]) == true) {
			//std::cout << "\tV\n" << NORMAL;
			for (int j = i; j < size - 1; j++)
				polys[j] = polys[j+1];
			size--;
			i--;
		} else {
			//std::cout << "\tX\n" << NORMAL;
		}
	}
#ifdef __PRT_SVM_I
	std::cout << "->" << size << std::endl;
#endif
	return true;
}
#endif

std::string Classifier::toString() const {
	std::ostringstream stm;
	if (size <= 0) {
		stm << "true";
		return stm.str();
	}
	stm << "(" << polys[0] << ") ";
	for (int i = 1; i < size; i++) 
		stm << cts[i] << " (" << polys[i] << ") ";
	return stm.str();
}

std::ostream& operator << (std::ostream& out, const Classifier& cs) {
	out << cs.toString();
	return out;
}
