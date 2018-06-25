/** @file polynomial.h
 *  @brief Defines the linear polynomial format and its solution format.
 *
 *  @author Li Jiaying
 *  @bug No known bugs. 
 */

#include "polynomial.h"

const double UPBOUND = pow(0.1, PRECISION);
static bool _roundoff(double x, double& roundx)
{
	double xx = x;
	if (std::abs(xx) <= UPBOUND) {
		roundx = 0;
		return true;
	}
	roundx = nearbyint(xx);
	if ((roundx  >= xx * (1 - UPBOUND) && roundx  <= xx * (1 + UPBOUND))
			|| (roundx  <= xx * (1 - UPBOUND) && roundx  >= xx * (1 + UPBOUND))) {
		return true;
	}
	return false;
}

static bool _roundoffPoly(Polynomial& p) {
	int size = p.getDims();
	std::vector<double> ret;
	for (int i = 0; i < size; i++) {
		ret.push_back(p.theta[i]);
		if (_roundoff(p.theta[i], ret[i]) == false) {
			ret.clear();
			return false;
		}
	}
	for (int i = 0; i < size; i++)
		p.theta[i] = ret[i];
	ret.clear();
	return true;
}

static bool scale(Polynomial& poly, double times) {
	if (times == 0) return false;
	//std::cout << poly.getDims() << "--";
	for (int i = 0; i < poly.getDims(); i++)
		poly[i] *= times;
	return true;
}

static int gcd(int a, int b) {
	if (a==1 || b==1)
		return 1;
	if (a==0 || b==0)
		return (a+b)>0? (a+b):1;
	if (a==b)
		return a;
	if (a<b) {
		int tmp = a;
		a = b;
		b = tmp;
	}
	return gcd(b, a%b);
}

static int ngcd(Polynomial& poly/*bool ignore_first = true*/) {
	for (int i = 0; i < poly.getDims(); i++) {
		if ((int)poly.theta[i] - poly.theta[i] != 0)
			return 1;
	}
	int coffgcd = std::abs((int)poly.theta[1]);
	for (int i = 2; i < poly.getDims(); i++) {
		if (coffgcd == 1)
			break;
		coffgcd = gcd(coffgcd, std::abs((int)poly.theta[i]));
	}
	return coffgcd;
}

std::string Polynomial::toString() const {
	std::ostringstream stm;
	stm << std::setprecision(16);
	bool firstplus = false;
	for (int j = 0; j < dims; j++) {
		if (theta[j] == 0) continue;
		if (firstplus == false) {
			firstplus = true;
			if (theta[j] < 0) stm << " -";
		} else {
			if (theta[j] < 0) stm << " - ";
			else stm << " + ";
		}
		if (j == 0) {
			stm << std::abs(theta[j]);
			continue;
		}
		if (std::abs(theta[j]) != 1)
			stm << std::abs(theta[j]) << "*";
		stm << variables[j];
	}
	stm << " >= 0";

	return stm.str();
}

std::ostream& operator<< (std::ostream& out, const Polynomial& poly) {
	out << std::setprecision(16);
	out << poly.toString();
	return out;
	return out;
}

Polynomial& Polynomial::operator=(Polynomial& rhs) {
	if (this == &rhs) { return *this; }
	setEtimes(rhs.getEtimes());
	for (int i = 0; i < rhs.getDims(); i++)
		theta[i] = rhs.theta[i];
	return *this;
}

bool Polynomial::operator==(const Polynomial& rhs) {
	if (dims != rhs.getDims())
		return false;
	for (int i = 0; i < rhs.getDims(); i++)
		if (theta[i] != rhs.theta[i])
			return false;
	return true;
}

Polynomial* Polynomial::roundoff() {
	Polynomial poly;
	this->roundoff(poly);
	*this = poly;
	return this;
}

bool Polynomial::alreadyRoundoff() {
	for (int i = 0; i < dims; i++)
		if (int(theta[i]) != theta[i]) 
			return false;
	return true;
}

int Polynomial::roundoff(Polynomial& e) {
	//std::cout << "ROUND OFF " << *this << " --> ";
	/*if (alreadyRoundoff() == true) {
		e = *this;
		return 0;
	}
	*/
	double max = 0;
	for (int i = 0; i < dims; i++) {
		if (std::abs(theta[i]) > max) {
			max = std::abs(theta[i]);
		}
	}
	double min = max;
	for (int i = 1; i < dims; i++) {
		if (std::abs(theta[i]) == 0) continue;
		if (std::abs(theta[i]) * pow(10, PRECISION) < max) {
			theta[i] = 0;
			continue;
		}
		if (std::abs(theta[i]) < min) {
			min = std::abs(theta[i]);
		}
	}


#ifdef __PRT_POLYNOMIAL
	std::cout << GREEN << "Before roundoff: " << *this;
	std::cout << " min=" << min << std::endl;
#endif

	e = *this;
	scale(e, 1.0/min);
	double scale_up = 2;
	//e.theta[0] = theta[0]/min;
	while(scale_up <= 100) {
		if (_roundoffPoly(e) == true)
			break;
		scale(e, (1.0 * scale_up)/(scale_up-1));
		scale_up++;
	}
	/*	int i;
		for (i = 0; i < dims; i++) {
			if (_roundoff(theta[i] / min, e.theta[i]) == false) {
				//std::cout << RED << "scale X10:" << GREEN << *this << std::endl;
				scale(*this, scale_up/(scale_up-1));
				scale_up++;
				break;
			} 
		}
		if (i >= dims)
			break;
	}
	*/
	if (scale_up > 100) {
		for (int i = 0; i < dims; i++) {
			_roundoff(e.theta[i], e.theta[i]);
		}
	}

	//*
	int poly_gcd = ngcd(e);
	e.theta[0] = floor(e.theta[0] / poly_gcd);
	if (poly_gcd > 1) {
		for (int i = 1; i < dims; i++) {
			e.theta[i] = e.theta[i] / poly_gcd;
		}
	}
	//*/
	//e.theta[0] = floor(e.theta[0]);
#ifdef __PRT_POLYNOMIAL
	std::cout << "\tAfter roundoff: " << e << NORMAL << std::endl;
#endif
	//std::cout << e << std::endl;
	return 0;
}

// Aggressively round off coefficients
int Polynomial::agressiveRoundoff(int threshold) {
	int aggressive = 1;
	for (int i = 1; i < dims; i++) {
		if (std::abs(theta[i]) <= threshold) {
			aggressive = 0;
			break;
		}
	}
	if (!aggressive) 
		return 0;

	double max = 0;
	for (int i = 0; i < dims; i++) {
		if (std::abs(theta[i]) > max) {
			max = std::abs(theta[i]);
		}
	}
	double min = max;
	for (int i = 1; i < dims; i++) {
		if (std::abs(theta[i]) == 0) continue;
		if (std::abs(theta[i]) * pow(10, PRECISION) < max) {
			theta[i] = 0;
			continue;
		}
		if (std::abs(theta[i]) < min) {
			min = std::abs(theta[i]);
		}
	}

	for (int i = 0; i < dims; i++) {
		theta[i] = nearbyint(theta[i] / min);
	}
	return 0;
}