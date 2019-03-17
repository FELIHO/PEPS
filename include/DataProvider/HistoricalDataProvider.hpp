#pragma once

#ifndef HistoricalDataProvider_HPP
#define HistoricalDataProvider_HPP

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "DataProvider.hpp"
#include "BlackScholesModel.hpp"
#include "ParameterEstimation.hpp"
#include "RandomGen.hpp"

using namespace std;


class HistoricalDataProvider : public DataProvider
{
public:

	char const *fileData_;
	int firstDateIndex_;

	HistoricalDataProvider();
	HistoricalDataProvider(const HistoricalDataProvider &HDP);
	HistoricalDataProvider& operator=(const HistoricalDataProvider &HDP);
	~HistoricalDataProvider();

	HistoricalDataProvider(char const *fileData,int firstDateIndex);
	//HistoricalDataProvider(RandomGen *rng, char const *fileData, PnlVect* dividend);

	PnlMat* getMarketData(double T, int nbTimeSteps, int nbRebalancementPerStep);
	PnlMat* getDailyMarketData(double T);
	PnlMat* getWeeklyMarketData(double T, int nbTimeSteps);
	PnlMat* getMonthlyMarketData(double T, int nbTimeSteps);

	PnlMat* multiplyChangeRate(PnlMat* marketData);
};

#endif
