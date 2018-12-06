#pragma once
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Option.hpp"
#define DLLEXP   __declspec( dllexport )

namespace Computations {
	class Performance : public Option {
	public:
		PnlVect *PayOff_coefficient;
		Performance();
		Performance(double T, int nbTimeSteps, int size, PnlVect *payoff_coefficient);
		Performance(const Performance &P);
		Performance& operator=(const Performance &P);
		~Performance();
		DLLEXP double payoff(const PnlMat *path);
	};
}
