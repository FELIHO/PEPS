#pragma once
#include "pch.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "DataProvider.hpp"

namespace Computations {

	class SimulatedDataProvider : public DataProvider
	{
	public:
		SimulatedDataProvider(PnlVect* spot, double sigma, double r , double rho, int nbDates, int timeEstimation);
    SimulatedDataProvider(PnlVect* spot, PnlVect* volatilities, double r , double rho, int nbDates, int timeEstimation);
    ~SimulatedDataProvider();
		SimulatedDataProvider(const SimulatedDataProvider& D);
		SimulatedDataProvider& operator=(const SimulatedDataProvider &D);
	};

}
