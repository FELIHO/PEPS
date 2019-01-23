#pragma once
#include "pch.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

namespace Computations {

	class SimulatedDataProvider
	{
	public:
    SimulatedDataProvider(PnlVect* spot, PnlVect* volatilities, double r , double rho, int nbDates, int timeEstimation);
    ~SimulatedDataProvider() {};
    int GetDataFeed(PnlMat *DataFeed_, int nbDates, int timeEstimation) = 0;
	};

}
