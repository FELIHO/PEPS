#pragma once

#ifndef DATAPROVIDER_HPP
#define DATAPROVIDER_HPP

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"


class DataProvider
{
public:

	~DataProvider() {};

	virtual PnlMat* getMarketData(double T, int nbTimeSteps, int nbRebalancementPerStep)= 0;
	virtual PnlMat* getDailyMarketData(double T)= 0;

};

#endif
