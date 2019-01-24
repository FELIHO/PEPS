#pragma once
#include "pch.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "MonteCarlo.hpp"

namespace Computations {

	class DataProvider
	{
	public:
		PnlMat *DataFeed_;
		int indexFirstSpot_;
		MonteCarlo *mc_;
		DataProvider();
    	~DataProvider();
		DataProvider(const DataProvider& D);
		DataProvider& operator=(const DataProvider &D);
	};

}
