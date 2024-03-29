#include "QuantoBasket.hpp"



QuantoBasket::QuantoBasket() {
	weights_ = pnl_vect_new();
	T_ = 0;
	nbTimeSteps_ = 0;
	size_ = 0;
	strike_ = 0;
	currency_ = pnl_vect_new();
}

QuantoBasket::QuantoBasket(const QuantoBasket &QB) {
	T_ = QB.T_;
	strike_ = QB.strike_;
	nbTimeSteps_ = QB.nbTimeSteps_;
	size_ = QB.size_;
	weights_ = pnl_vect_copy(QB.weights_);
	currency_ = pnl_vect_copy(QB.currency_);
}

QuantoBasket& QuantoBasket::operator=(const QuantoBasket &C) {
	T_ = C.T_;
	strike_ = C.strike_;
	nbTimeSteps_ = C.nbTimeSteps_;
	size_ = C.size_;
	weights_ = C.weights_;
	currency_ = C.currency_;
	return *this;
}

QuantoBasket::~QuantoBasket()
{
	pnl_vect_free(&weights_);
	pnl_vect_free(&currency_);
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

void QuantoBasket::setCurrency(PnlVect * currency){
	currency_ = pnl_vect_copy(currency);
}

double QuantoBasket::payoff(const PnlMat *path) {
	PnlMat* assets_path = pnl_mat_create(path->m,size_);
	pnl_mat_extract_subblock(assets_path, path, 0,path->m, 0,size_);

	PnlMat* ExR_path = pnl_mat_create(path->m,path->n-size_);
	pnl_mat_extract_subblock(ExR_path, path, 0,path->m, size_,path->n-size_);

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
	last_date_change_rate = pnl_vect_create(ExR_path->n);

	// Charging the last row
	pnl_mat_get_row(last_date, assets_path, nbTimeSteps_);
    pnl_mat_get_row(last_date_change_rate, ExR_path, nbTimeSteps_);
    for(int i = 0; i < last_date->size; i++) {
		res += pnl_vect_get(weights_, i)*pnl_vect_get(last_date, i)*pnl_vect_get(last_date_change_rate, pnl_vect_get(currency_, i));
    }

	res = sign*res - strike_;

	// Deleting last_date
	pnl_vect_free(&last_date);
	pnl_vect_free(&last_date_change_rate);

	if (res >= 0)
	{
		return res;
	}
	else
	{
		return 0.0;
	}
}
