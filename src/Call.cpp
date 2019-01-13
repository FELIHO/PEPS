#include "Call.hpp"
using namespace Computations;



Call::Call() : Basket() {

}


Call::Call(double T, double strike_) : Basket(T, 1, 1,  strike_, pnl_vect_create_from_scalar(1, 1.0)) {}



Call::Call(const Call &C) {
	T_ = C.T_;
	nbTimeSteps_ = C.nbTimeSteps_;
	size_ = C.size_;
	strike_ = C.strike_;
	PayOff_coefficient = pnl_vect_copy(C.PayOff_coefficient);
}


Call& Call::operator=(const Call &C) {
	T_ = C.T_;
	nbTimeSteps_ = C.nbTimeSteps_;
	size_ = C.size_;
	strike_ = C.strike_;
	PayOff_coefficient = C.PayOff_coefficient;
	return *this;
}


Call::~Call() {
	pnl_vect_free(&PayOff_coefficient);
}
