#include "Call.hpp"



Call::Call(){
	weights_ = pnl_vect_create_from_scalar(1, 1.0);
  	T_ = 0;
  	nbTimeSteps_ = 1;
  	size_ = 1;
  	strike_ = 0;
}

Call::Call(const Call &C) {
	T_ = C.T_;
	strike_ = C.strike_;
	nbTimeSteps_ = C.nbTimeSteps_;
	size_ = C.size_;
	weights_ = pnl_vect_copy(C.weights_);
}

Call& Call::operator=(const Call &C) {
	T_ = C.T_;
	strike_ = C.strike_;
	nbTimeSteps_ = C.nbTimeSteps_;
	size_ = C.size_;
	weights_ = C.weights_;
	return *this;
}

Call::~Call()
{
  pnl_vect_free(&weights_);
}

Call::Call(double T, double strike){
	weights_ = pnl_vect_create_from_scalar(1, 1.0);
  	T_ = T;
  	nbTimeSteps_ = 1;
  	size_ = 1;
  	strike_ = strike;
}

Call* Call::clone() {
	return (new Call(*this));
}
