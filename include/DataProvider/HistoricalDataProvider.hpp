#pragma once
#include "pch.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "DataProvider.hpp"

namespace Computations {

	class HistoricalDataProvider : public DataProvider
	{
	public:
    HistoricalDataProvider(const PnlMat* historicalData, double r, int nbDates, int timeEstimation, int indexFirstSpot);
    ~HistoricalDataProvider();
		HistoricalDataProvider(const HistoricalDataProvider& D);
		HistoricalDataProvider& operator=(const HistoricalDataProvider &D);
	};

}
