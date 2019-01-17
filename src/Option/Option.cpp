#include "Option.hpp"
#include "pch.h"
using namespace Computations;



Option::Option(){
  	T_ = 0;
  	nbTimeSteps_ = 1;
  	size_ = 0;
}

Option::Option(const Option &C) {
	T_ = C.T_;
	nbTimeSteps_ = C.nbTimeSteps_;
	size_ = C.size_;
}

Option& Option::operator=(const Option &C) {
	T_ = C.T_;
	nbTimeSteps_ = C.nbTimeSteps_;
	size_ = C.size_;
	return *this;
}

Option::~Option()
{
}

Option* Option::clone() {
	return (new Option(*this));
}

double Option::payoff(const PnlMat *path)
{
return 0;
}

double Option::payoff(const PnlMat *path, const PnlMat *pathChangeRate, const PnlVect *currency)
{
return 0;
}
