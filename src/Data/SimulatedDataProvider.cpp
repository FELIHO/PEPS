#pragma once
#include "pch.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "SimulatedDataProvider.hpp"
#include "BlackScholesModel.hpp"
using namespace Computations;



SimulatedDataProvider::SimulatedDataProvider(PnlVect* spot, PnlVect* volatilities, double r , double rho, int nbDates, int timeEstimation)
{
	indexFirstSpot_ = timeEstimation;
	BlackScholesModel *bsm = new BlackScholesModel(spot->size, r , rho, volatilities, spot);
	PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	bsm->asset(DataFeed_, (nbDates+0.0)/252, timeEstimation + nbDates, rng);
	pnl_rng_free(&rng);
}

SimulatedDataProvider::~SimulatedDataProvider(){
	pnl_mat_free(&DataFeed_);
}
