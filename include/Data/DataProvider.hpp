#pragma once
#include "pch.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

namespace Computations {

	class DataProvider
	{
	public:
    virtual ~DataProvider() {};
    virtual int *GetDataFeed(PnlMat *DataFeed_, int nbDates, int timeEstimation) = 0;
	};

}
