#pragma once

#ifndef KOZEI_HPP
#define KOZEI_HPP


#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Option.hpp"


class Kozei : public Option {
public:
	double inv_init_; //investissement initial
	double r_CHF = 0.1;
	double r_USD = 0.4;
	double r_JPY = 0.2;
	double r_GBP = 0.3;
	double r_BRL = 0.5;
	Kozei();
	Kozei(double inv_init);
	Kozei(const Kozei &K);
	Kozei& operator=(const Kozei &K);
	~Kozei();
	double payoff(const PnlMat *path);
	virtual Kozei* clone();
	double payoff_with_ExR(const PnlMat *path);
};

#endif
