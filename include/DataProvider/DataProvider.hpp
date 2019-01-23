#pragma once
#include "pch.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

namespace Computations {

	class DataProvider
	{
	public:
		PnlMat *DataFeed_;
		int indexFirstSpot_;
		DataProvider();
    	~DataProvider();
		DataProvider(const DataProvider& D);
		DataProvider& operator=(const DataProvider &D);
	};

}
