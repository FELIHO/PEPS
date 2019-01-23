#pragma once
#include "pch.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

namespace Computations {

	class DataProvider
	{
	public:
<<<<<<< HEAD:include/DataProvider/DataProvider.hpp

    virtual ~DataProvider() {};

    virtual void *GetDataFeed(PnlMat *DataFeed_, int nbDates, int estimationWindow) = 0; 

=======
		PnlMat *DataFeed_;
		int indexFirstSpot_;
		DataProvider();
    virtual ~DataProvider() {};
>>>>>>> 120b6e8215906b8317e8cf19dd4210cb496eaad6:include/Data/DataProvider.hpp
	};

}
