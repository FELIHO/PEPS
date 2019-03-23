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

SimulatedDataProvider::SimulatedDataProvider(RandomGen *rng, double r , double rho, PnlVect* sigma, PnlVect* spot){
	mod_ =  new BlackScholesModel(spot->size, r , rho, sigma, spot);
	rng_ = rng->clone();
}

SimulatedDataProvider::SimulatedDataProvider(RandomGen *rng, PnlVect* trend , PnlMat *rho, PnlVect* sigma, PnlVect* spot, PnlVect* dividend)
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
	int H = T*Tools::NumberOfDaysPerYear;
	PnlMat* marketData = pnl_mat_new();
	mod_->simul_market(marketData, T, T*Tools::NumberOfDaysPerYear, rng_);
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
