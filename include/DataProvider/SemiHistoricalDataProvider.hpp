#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "DataProvider.hpp"
#include "BlackScholesModel.hpp"
#include "ParameterEstimation.hpp"
#include "RandomGen.hpp"

using namespace std; 


class SemiHistoricalDataProvider : public DataProvider
{
public:

	BlackScholesModel *mod_;
	RandomGen *rng_;
	char const *fileData_;

	SemiHistoricalDataProvider();
	SemiHistoricalDataProvider(const SemiHistoricalDataProvider &SHDP);
	SemiHistoricalDataProvider& operator=(const SemiHistoricalDataProvider &SHDP);
	~SemiHistoricalDataProvider();

	SemiHistoricalDataProvider(RandomGen *rng, char const *fileData);
	//SemiHistoricalDataProvider(RandomGen *rng, char const *fileData, PnlVect* dividend);
	
	PnlMat* getMarketData(double T, int nbTimeSteps, int nbRebalancementPerStep);
	PnlMat* getDailyMarketData(double T);
	PnlMat* getWeeklyMarketData(double T, int nbTimeSteps);
	PnlMat* getMonthlyMarketData(double T, int nbTimeSteps);
};
