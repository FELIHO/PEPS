#pragma once
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Option.hpp"
#define DLLEXP   __declspec( dllexport )

namespace Computations {
	class Asian : public Option {
	public:
		double strike_;
		PnlVect *PayOff_coefficient;
		Asian();
		Asian(double T, int nbTimeSteps, int size, double strike, PnlVect *payOff_coefficient);
		Asian(const Asian &A);
		Asian& operator=(const Asian &A);
		~Asian();
		DLLEXP double payoff(const PnlMat *path);
	};
}

