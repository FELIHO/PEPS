#pragma once
#include "pch.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

namespace Computations {

	class HistoricalDataProvider
	{
	public:

    HistoricalDataProvider();
    HistoricalDataProvider(const HistoricalDataProvider &HDC);
    HistoricalDataProvider& operator=(const HistoricalDataProvider &HDC);
    ~HistoricalDataProvider();

    void *GetDataFeed(PnlMat *DataFeed_, int nbDates, int estimationWindow) ; 

	};



}