#pragma once
#include "pch.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

namespace Computations {

	class DataProvider
	{
	public:
    /*
    * Matrice contenant toutes les dates
    */
  	PnlMat *DataFeed_;

    virtual ~DataProvider() {};

    virtual int *GetDataFeed(PnlMat *DataFeed_, int nbDates) = 0; 

	};



}
