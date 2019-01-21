#include "Option/QuantoBasket.hpp"
#include "pch.h"
using namespace Computations;



QuantoBasket::QuantoBasket() {
	weights_ = pnl_vect_create_from_scalar(1, 1.0);
	T_ = 0;
	nbTimeSteps_ = 1;
	size_ = 1;
	strike_ = 0;
}

QuantoBasket::QuantoBasket(const QuantoBasket &C) {
	T_ = C.T_;
	strike_ = C.strike_;
	nbTimeSteps_ = C.nbTimeSteps_;
	size_ = C.size_;
	weights_ = pnl_vect_copy(C.weights_);
}

QuantoBasket& QuantoBasket::operator=(const QuantoBasket &C) {
	T_ = C.T_;
	strike_ = C.strike_;
	nbTimeSteps_ = C.nbTimeSteps_;
	size_ = C.size_;
	weights_ = C.weights_;
	return *this;
}

QuantoBasket::~QuantoBasket()
{
	pnl_vect_free(&weights_);
}

QuantoBasket::QuantoBasket(double T, int nbTimeSteps, int size, double strike, PnlVect *weights) {
	weights_ = pnl_vect_copy(weights);
	T_ = T;
	nbTimeSteps_ = nbTimeSteps;
	size_ = size;
	strike_ = strike;
}

QuantoBasket* QuantoBasket::clone() {
	return (new QuantoBasket(*this));
}

double QuantoBasket::payoff(const PnlMat *path, const PnlMat *pathChangeRate, const PnlVect * currency) {
    assert(size_==currency->size);

	double res = 0.0;

	// For a Call basket sign=+1, for a Put basket sign=-1
	double sign = 1;
	if (strike_ < 0)
	{
		sign = -1;
	}

	// Creating a vector to contain the final row of the 'path' matrix
	PnlVect *last_date;
	last_date = pnl_vect_create(size_);
    PnlVect *last_date_change_rate;
	last_date_change_rate = pnl_vect_create(pathChangeRate->n);

	// Charging the last row
	pnl_mat_get_row(last_date, path, nbTimeSteps_);
    pnl_mat_get_row(last_date_change_rate, pathChangeRate, nbTimeSteps_);
    for(int i = 0; i < last_date->size; i++) {
        pnl_vect_set(last_date, i, (pnl_vect_get(last_date, i) - strike_) * pnl_vect_get(last_date_change_rate, pnl_vect_get(currency, i)));
    }

	res = (sign * pnl_vect_scalar_prod(weights_, last_date));

	// Deleting last_date
	pnl_vect_free(&last_date);

	if (res >= 0)
	{
		return res;
	}
	else
	{
		return 0.0;
	}
}