#include "Option/QuantoCall.hpp"
#include "pch.h"
using namespace Computations;



QuantoCall::QuantoCall(){
	weights_ = pnl_vect_create_from_scalar(1, 1.0);
  	T_ = 0;
  	nbTimeSteps_ = 1;
  	size_ = 1;
  	strike_ = 0;
}

QuantoCall::QuantoCall(const QuantoCall &C) {
	T_ = C.T_;
	strike_ = C.strike_;
	nbTimeSteps_ = C.nbTimeSteps_;
	size_ = C.size_;
	weights_ = pnl_vect_copy(C.weights_);
}

QuantoCall& QuantoCall::operator=(const QuantoCall &C) {
	T_ = C.T_;
	strike_ = C.strike_;
	nbTimeSteps_ = C.nbTimeSteps_;
	size_ = C.size_;
	weights_ = C.weights_;
	return *this;
}

QuantoCall::~QuantoCall()
{
  pnl_vect_free(&weights_);
}

QuantoCall::QuantoCall(double T, double strike){
	weights_ = pnl_vect_create_from_scalar(1, 1.0);
  	T_ = T;
  	nbTimeSteps_ = 1;
  	size_ = 1;
  	strike_ = strike;
}

QuantoCall* QuantoCall::clone() {
	return (new QuantoCall(*this));
}