#pragma once

#ifndef KOZEI_HPP
#define KOZEI_HPP


#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Option.hpp"
#include "Tools.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>


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
	Kozei* clone();
	PnlVect* Foreign_ZC(int number_of_dates,double rate);
	PnlMat* return_path_matrix(PnlMat *const assets_path,const PnlMat* ExR_path);
	double payoff(const PnlMat *path);
	double payoff_without_ExR(const PnlMat *path);
	double payoff_with_ExR(const PnlMat *path);
};

#endif
