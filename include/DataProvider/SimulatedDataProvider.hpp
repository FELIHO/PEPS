#pragma once

#ifndef SIMULATEDDATAPROVIDER_HPP
#define SIMULATEDDATAPROVIDER_HPP

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "DataProvider.hpp"
#include "BlackScholesModel.hpp"
#include "RandomGen.hpp"

class SimulatedDataProvider : public DataProvider
{
public:

	BlackScholesModel *mod_;
	RandomGen *rng_;

	SimulatedDataProvider();
	SimulatedDataProvider(const SimulatedDataProvider &DP);
	SimulatedDataProvider& operator=(const SimulatedDataProvider &DP);
	~SimulatedDataProvider();

	SimulatedDataProvider(RandomGen *rng, PnlVect* spot, double r , double rho, double sigma);
	SimulatedDataProvider(RandomGen *rng, PnlVect* spot, PnlVect* trend , PnlMat* rho ,PnlVect* sigma, PnlVect* dividend);

	PnlMat* getMarketData(double T, int nbTimeSteps, int nbRebalancementPerStep);
	PnlMat* getDailyMarketData(double T);
	PnlMat* getWeeklyMarketData(double T, int nbTimeSteps);
	PnlMat* getMonthlyMarketData(double T, int nbTimeSteps);
};

#endif
