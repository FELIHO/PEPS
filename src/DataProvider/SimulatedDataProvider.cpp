#include "SimulatedDataProvider.hpp"



SimulatedDataProvider::SimulatedDataProvider() {
  mod_ = new BlackScholesModel();
  rng_ = new FakeRnd(0.1);
}


SimulatedDataProvider::SimulatedDataProvider(const SimulatedDataProvider &SDP) {
  mod_ = new BlackScholesModel(*SDP.mod_);
  rng_ = SDP.rng_->clone();
}

SimulatedDataProvider& SimulatedDataProvider::operator= (const SimulatedDataProvider &SDP) {
  mod_ = SDP.mod_;
  rng_ = SDP.rng_;
  return *this;
}

SimulatedDataProvider::~SimulatedDataProvider(){
	delete(mod_);
	delete(rng_);
}

SimulatedDataProvider::SimulatedDataProvider(RandomGen *rng, PnlVect* spot, double r , double rho, double sigma){
	PnlVect* vectSigma = pnl_vect_create_from_scalar(spot->size, sigma);
	mod_ =  new BlackScholesModel(spot->size, r , rho, vectSigma, spot);
	rng_ = rng->clone();
	pnl_vect_free(&vectSigma);
}

SimulatedDataProvider::SimulatedDataProvider(RandomGen *rng, PnlVect* spot,  PnlVect* trend , PnlMat *rho, PnlVect* sigma, PnlVect* dividend)
{
	mod_ =  new BlackScholesModel(spot->size, pnl_vect_sum(trend)/(trend->size) , rho, sigma, spot, trend, dividend );
	rng_ = rng->clone();
}


PnlMat* SimulatedDataProvider::getMarketData(double T, int nbTimeSteps, int nbRebalancementPerStep){
	int H = nbRebalancementPerStep*nbTimeSteps;
	PnlMat* marketData = pnl_mat_new();
	mod_->simul_market(marketData, T, H, rng_);
	return marketData;
}

PnlMat* SimulatedDataProvider::getDailyMarketData(double T){
	int H = T*260;
	PnlMat* marketData = pnl_mat_new();
	mod_->simul_market(marketData, T, T*260, rng_);
	return marketData;
}

PnlMat* SimulatedDataProvider::getWeeklyMarketData(double T, int nbTimeSteps){
	int H = T*52;
	H -=  H%nbTimeSteps;
	PnlMat* marketData = pnl_mat_new();
	mod_->simul_market(marketData, T, H, rng_);
	return marketData;
}

PnlMat* SimulatedDataProvider::getMonthlyMarketData(double T, int nbTimeSteps){
	PnlMat* marketData = pnl_mat_new();
	int H = T*12;
	H -=  H%nbTimeSteps;
	mod_->simul_market(marketData, T, H, rng_);
	return marketData;
}
