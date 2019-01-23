#pragma once
#include "pch.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

namespace Computations {

	class SimulatedDataProvider
	{
	public:

    SimulatedDataProvider();
    SimulatedDataProvider(const SimulatedDataProvider &HDC);
    SimulatedDataProvider& operator=(const SimulatedDataProvider &HDC);
    ~SimulatedDataProvider();

    void *GetDataFeed(PnlMat *DataFeed_, int nbDates, int estimationWindow) ; 

	};



}