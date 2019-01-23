#include "SimulatedDataProvider.hpp"
#include "BlackScholesModel.hpp"

using namespace Computations;


SimulatedDataProvider::SimulatedDataProvider(PnlVect* spot, double sigma, double r , double rho, int nbDates, int timeEstimation){
	indexFirstSpot_ = timeEstimation;
	PnlVect* volatilities = pnl_vect_create_from_scalar(spot->size, sigma);
	BlackScholesModel *bsm = new BlackScholesModel(spot->size, r , rho, volatilities, spot);
	PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	bsm->asset(DataFeed_, (nbDates+ timeEstimation + 0.0)/252.6, timeEstimation + nbDates, rng);
	pnl_rng_free(&rng);
	delete(bsm);
}

SimulatedDataProvider::SimulatedDataProvider(PnlVect* spot, PnlVect* volatilities, double r , double rho, int nbDates, int timeEstimation)
{
	indexFirstSpot_ = timeEstimation;
	BlackScholesModel *bsm = new BlackScholesModel(spot->size, r , rho, volatilities, spot);
	PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	bsm->asset(DataFeed_, (nbDates+0.0)/252.6, timeEstimation + nbDates, rng);
	pnl_rng_free(&rng);
	delete(bsm);
}

SimulatedDataProvider::~SimulatedDataProvider(){
	pnl_mat_free(&DataFeed_);
}

SimulatedDataProvider::SimulatedDataProvider(const SimulatedDataProvider& D) {
  DataFeed_ = pnl_mat_copy(D.DataFeed_);
  indexFirstSpot_ = D.indexFirstSpot_;
}


SimulatedDataProvider& SimulatedDataProvider::operator=(const SimulatedDataProvider &D) {
  DataFeed_ = D.DataFeed_;
  indexFirstSpot_ = D.indexFirstSpot_;
	return *this;
}
