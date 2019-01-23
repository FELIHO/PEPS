#pragma once
#include "pch.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "HistoricalDataProvider.hpp"
#include "BlackScholesModel.hpp"
using namespace Computations;



HistoricalDataProvider::HistoricalDataProvider(PnlMat* historicalData,  double r, int nbDates, int timeEstimation)
{
	indexFirstSpot_ = timeEstimation;
	BlackScholesModel *bsm = new BlackScholesModel(spot->size, r , rho, volatilities, spot);
	PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	bsm->asset(DataFeed_, (nbDates+0.0)/252, timeEstimation + nbDates, rng);
	pnl_rng_free(&rng);
}

HistoricalDataProvider::~HistoricalDataProvider(){
	pnl_mat_free(&DataFeed_);
}
