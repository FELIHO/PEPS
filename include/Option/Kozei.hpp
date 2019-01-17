#pragma once
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Option/Option.hpp"
#define DLLEXP   __declspec( dllexport )

namespace Computations {
	class Kozei : public Option {
	public:
		double inv_init_; //investissement initial
		int T = 8;
		int nbTimeSteps = 182 / 365;
		int size = 30;
		Kozei();
		Kozei(double inv_init);
		Kozei(const Kozei &K);
		Kozei& operator=(const Kozei &K);
		~Kozei();
		DLLEXP double payoff(const PnlMat *path);
	};
}