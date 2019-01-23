#pragma once
#include "pch.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

namespace Computations {

	class HistoricalDataProvider : public DataProvider
	{
	public:
    HistoricalDataProvider(const PnlMat* allData);
    virtual ~HistoricalDataProvider() {};
    virtual int *GetDataFeed(PnlMat *DataFeed_, int nbDates, int timeEstimation) = 0;
	};

}
