#pragma once
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Option.hpp"

#define DLLEXP   __declspec( dllexport )
namespace Computations {
	class Basket : public Option {
	public:
		double strike_;
		PnlVect *PayOff_coefficient;
		Basket();
		Basket(double T, int nbTimeSteps, int size, double strike_, PnlVect *payoff_coefficient);
		Basket(const Basket &B);
		Basket& operator=(const Basket &B);
		~Basket();
		DLLEXP double payoff(const PnlMat *path);
	};
}