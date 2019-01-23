#include "HistoricalDataProvider.hpp"
#include "BlackScholesModel.hpp"
#include "ParameterEstimation.hpp"
#include "DataSelecter.hpp"
using namespace Computations;


//2021 jour
HistoricalDataProvider::HistoricalDataProvider(PnlMat* historicalData, double r, int nbDates, int timeEstimation)
{
	indexFirstSpot_ = timeEstimation;
	ParameterEstimation pe;
	DataSelecter ds;
	PnlVect* volatilities = pe.getVolatilitiesVector(historicalData);
	PnlMat* rho = pe.getCorrelationMatrix(historicalData);
	PnlVect* spot = pnl_vect_new();
	pnl_mat_get_row(spot, historicalData, historicalData->m - 1);
	BlackScholesModel *bsm = new BlackScholesModel(spot->size, r , rho, volatilities, spot);
	PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	DataFeed_ = pnl_mat_create(nbDates + timeEstimation, historicalData->n);
	PnlMat* pastData = pnl_mat_new();
	double t = 0;
	//computeTvalue(const PnlVectInt *dateIndexes, const int DateZero, const int DateT, const double T);


	bsm->asset(DataFeed_, t,(nbDates+0.0)/252.6, timeEstimation + nbDates, rng, pastData);
	pnl_vect_free(&volatilities);
	pnl_mat_free(&rho);
	pnl_mat_free(&pastData);
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
}
