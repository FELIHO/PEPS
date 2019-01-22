#include "Option/QuantoCall.hpp"
#include "pch.h"
using namespace Computations;



QuantoCall::QuantoCall(){
	weights_ = pnl_vect_new();
	T_ = 0;
	nbTimeSteps_ = 0;
	size_ = 0;
	strike_ = 0;
}

QuantoCall::QuantoCall(const QuantoCall &QC) {
	T_ = QC.T_;
	strike_ = QC.strike_;
	nbTimeSteps_ = QC.nbTimeSteps_;
	size_ = QC.size_;
	weights_ = pnl_vect_copy(QC.weights_);
}

QuantoCall& QuantoCall::operator=(const QuantoCall &QC) {
	T_ = QC.T_;
	strike_ = QC.strike_;
	nbTimeSteps_ = QC.nbTimeSteps_;
	size_ = QC.size_;
	weights_ = QC.weights_;
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