#include "HedgePortfolio.hpp"
using namespace std;


HedgePortfolio::~HedgePortfolio(){
  delete(monteCarlo_);
  pnl_mat_free(&past_);
  pnl_vect_free(&delta_);
  pnl_vect_free(&S_current);
}

HedgePortfolio::HedgePortfolio(const HedgePortfolio &HedgePortfolio){

  //currentDate_  = HedgePortfolio.currentDate_;
  currentIndexRebalancement_ = HedgePortfolio.currentIndexRebalancement_;
  monteCarlo_ = new MonteCarlo(*HedgePortfolio.monteCarlo_);
  prix_ = HedgePortfolio.prix_;
  ic_ = HedgePortfolio.ic_;
  past_ = pnl_mat_copy(HedgePortfolio.past_);
  delta_ = pnl_vect_copy(HedgePortfolio.delta_);
  S_current = pnl_vect_copy(HedgePortfolio.S_current);
  investTauxSansRisque_ = HedgePortfolio.investTauxSansRisque_;

}

HedgePortfolio::HedgePortfolio(PnlMat* marketData, MonteCarlo* monteCarlo){
  //currentDate_ = 0;
  currentIndexRebalancement_= 0;
  monteCarlo_ = new MonteCarlo(*monteCarlo);
  H_ = marketData->m;
  prix_ = 0;
  ic_ = 0;

  past_ = pnl_mat_create(monteCarlo_->opt_->nbTimeSteps_+1,monteCarlo_->opt_->size_);

  updatePast(marketData,0,0);

  PnlMat pastTronq = pnl_mat_wrap_mat_rows(past_,0,0);

  monteCarlo_->price(&pastTronq, 0, prix_, ic_);

  delta_ = pnl_vect_new();
  monteCarlo->delta(&pastTronq,0,delta_);

  S_current = pnl_vect_new();
  pnl_mat_get_row(S_current,marketData,0);
  investTauxSansRisque_ = prix_ - pnl_vect_scalar_prod(delta_, S_current);

}

void HedgePortfolio::updateCompo(PnlMat* marketData){
  double currentDate = currentIndexRebalancement_ * (monteCarlo_->opt_->T_ / H_);
  //int indexRebalancement = currentDate_/(monteCarlo_->opt_->T_/H_);
  cout << currentIndexRebalancement_ << endl;
  cout << currentDate << endl;
  cout << (currentIndexRebalancement_*monteCarlo_->opt_->nbTimeSteps_)/H_ << endl;
  int indexToUpdate = ceil ( (currentIndexRebalancement_*monteCarlo_->opt_->nbTimeSteps_)/H_ );
  cout << indexToUpdate << endl;
  updatePast(marketData,currentIndexRebalancement_,indexToUpdate);

  PnlMat pastTronq = pnl_mat_wrap_mat_rows(past_,0,indexToUpdate);

  monteCarlo_->price(&pastTronq, currentDate, prix_, ic_);

  PnlVect* delta_anterieure = pnl_vect_new();
  pnl_vect_clone(delta_anterieure, delta_);
  monteCarlo_->delta(&pastTronq,currentDate,delta_);


  pnl_mat_get_row(S_current,marketData,currentIndexRebalancement_);

  PnlVect * differenceDelta = pnl_vect_new();
  pnl_vect_clone(differenceDelta, delta_);
  pnl_vect_minus_vect(differenceDelta, delta_anterieure);

  double val = pnl_vect_scalar_prod(differenceDelta, S_current);
  double expo = exp(monteCarlo_->mod_->r_*monteCarlo_->opt_->T_/H_);

  investTauxSansRisque_ = investTauxSansRisque_*expo - val;

  /*liberation de la memoire*/
  pnl_vect_free(&differenceDelta);
  pnl_vect_free(&delta_anterieure);
}


double HedgePortfolio::HedgeError(PnlMat * marketData){
  currentIndexRebalancement_ = 1;
  while (currentIndexRebalancement_ < H_) {
	updateCompo(marketData);
        currentIndexRebalancement_ += 1;
  }
  double payoff = monteCarlo_->opt_->payoff(past_);


  double PL = investTauxSansRisque_ + pnl_vect_scalar_prod(delta_, S_current)-payoff;
  return PL;
}

void HedgePortfolio::updatePast(PnlMat* marketData,int indexRebalancement,int indexToUpdate){
  PnlVect V = pnl_vect_wrap_mat_row(marketData, indexRebalancement);
  pnl_mat_set_row(past_, &V, indexToUpdate);
}
