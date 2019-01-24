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
<<<<<<< HEAD
		SimulatedDataProvider(PnlVect* spot, double sigma, double r , double rho, int nbDates, int timeEstimation);
    	SimulatedDataProvider(PnlVect* spot, PnlVect* volatilities, double r , double rho, int nbDates, int timeEstimation);
    	~SimulatedDataProvider();
=======
		SimulatedDataProvider(PnlVect* spot, double sigma, double r , double rho, int nbDates);
    SimulatedDataProvider(PnlVect* spot, PnlVect* volatilities, double r , double rho, int nbDates);
    ~SimulatedDataProvider();
>>>>>>> c5994f62565596eb7613c54e4465c5a83fcc9a25
		SimulatedDataProvider(const SimulatedDataProvider& D);
		SimulatedDataProvider& operator=(const SimulatedDataProvider &D);
	};

}
