#include "HistoricalDataProvider.hpp"
#include "BlackScholesModel.hpp"
#include "ParameterEstimation.hpp"
#include "DataSelecter.hpp"
#include <iostream>
using namespace Computations;
using namespace std;



//2021 jour
HistoricalDataProvider::HistoricalDataProvider(const PnlMat* historicalData, double r, int nbDates, int timeEstimation, int indexFirstSpot)
{
	indexFirstSpot_ = indexFirstSpot;
	ParameterEstimation pe;
	PnlVect* volatilities = pe.getVolatilitiesVector(historicalData);
	PnlMat* rho = pe.getCorrelationMatrix(historicalData);
	PnlVect* spot = pnl_vect_new();
	pnl_mat_get_row(spot, historicalData, historicalData->m - 1);
	std::cout << "SpotSize : " << spot->size << endl;
	BlackScholesModel *bsm = new BlackScholesModel(spot->size, r , rho, volatilities, spot);
	PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	DataFeed_ = pnl_mat_create(nbDates + timeEstimation, historicalData->n);
	PnlMat* pastData = pnl_mat_new();
	pnl_mat_extract_subblock(pastData, historicalData, indexFirstSpot, historicalData->m - 1, 0, historicalData->n);
	std::cout << "BSM : " << bsm->size_ << endl;
	std::cout << "HistoricalData : " << historicalData->n << endl;
	std::cout << "DataFeed_ : " << DataFeed_->n << endl;
	double t = (pastData->m - timeEstimation + 0.0)/nbDates;
	bsm->asset(DataFeed_, t,(nbDates+0.0)/252.6, timeEstimation + nbDates, rng, pastData);
	pnl_vect_free(&volatilities);
	pnl_mat_free(&rho);
	pnl_rng_free(&rng);
	delete(bsm);
}

HistoricalDataProvider::~HistoricalDataProvider(){
	pnl_mat_free(&DataFeed_);
}

HistoricalDataProvider::HistoricalDataProvider(const HistoricalDataProvider& D) {
  DataFeed_ = pnl_mat_copy(D.DataFeed_);
  indexFirstSpot_ = D.indexFirstSpot_;
}

HistoricalDataProvider& HistoricalDataProvider::operator=(const HistoricalDataProvider &D) {
  DataFeed_ = D.DataFeed_;
  indexFirstSpot_ = D.indexFirstSpot_;
	return *this;
}
