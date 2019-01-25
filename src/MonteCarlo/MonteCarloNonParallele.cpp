
#include "MonteCarlo.hpp"

using namespace std;

/* Constructeur par défault */
MonteCarlo::MonteCarlo(){
	mod_ = new BlackScholesModel();
	opt_ = new Call();
	rng_ = new FakeRnd(0.2);
  fdStep_ = 0;
  nbSamples_= 0;
 }

MonteCarlo::MonteCarlo(const MonteCarlo &MC){
  mod_ = new BlackScholesModel(*MC.mod_);
  opt_ = MC.opt_->clone();
	rng_ = MC.rng_->clone();
  fdStep_ = MC.fdStep_;
  nbSamples_ = MC.nbSamples_;
}

MonteCarlo& MonteCarlo::operator= (const MonteCarlo &MC) {
  mod_ = MC.mod_;
  opt_ = MC.opt_;
  rng_ = MC.rng_;
  fdStep_ = MC.fdStep_;
  nbSamples_ = MC.nbSamples_;
  return *this;
}

MonteCarlo::~MonteCarlo() {
  delete(mod_);
  delete(opt_);
  delete(rng_);
}

MonteCarlo::MonteCarlo(BlackScholesModel *mod, Option *opt, RandomGen *rng, double fdStep, int nbSamples)
{
  mod_ = new BlackScholesModel(*mod);
  opt_ = opt->clone();
	rng_ = rng->clone();
  fdStep_ = fdStep;
  nbSamples_ = nbSamples;
}


void MonteCarlo::price(double &prix, double &ic)
{
  double payoff;
  double sommePayoff = 0;
  double sommePayoffCarre = 0;
  PnlMat *pathCourant = pnl_mat_create(opt_->nbTimeSteps_+1, opt_->size_);

  for (int i = 0; i < nbSamples_; i++) {
    mod_->asset(pathCourant, opt_->T_, opt_->nbTimeSteps_, rng_);
    payoff = opt_->payoff(pathCourant);
    sommePayoff += payoff;
    sommePayoffCarre += payoff*payoff;
  }
  double moyennePayoff = sommePayoff/nbSamples_;
  double moyennePayoffCarre = sommePayoffCarre/nbSamples_;

  double ksiCarreM = exp(-2*mod_->r_*opt_->T_)*(moyennePayoffCarre-moyennePayoff*moyennePayoff);
  ic = 1.96*sqrt(ksiCarreM/nbSamples_)*2;
  prix = exp(-mod_->r_*opt_->T_)*moyennePayoff;

  pnl_mat_free(&pathCourant);

}

void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &ic)
{
  double payoff;
  double sommePayoff = 0;
  double sommePayoffCarre = 0;
  PnlMat *pathCourant = pnl_mat_create(opt_->nbTimeSteps_+1, opt_->size_);

  for (int i = 0; i < nbSamples_; i++) {
    mod_->asset(pathCourant, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
    payoff = opt_->payoff(pathCourant);
    sommePayoff += payoff;
    sommePayoffCarre += payoff*payoff;
  }
  double moyennePayoff = sommePayoff/nbSamples_;
  double moyennePayoffCarre = sommePayoffCarre/nbSamples_;

  double ksiCarreM = exp(-2*mod_->r_*opt_->T_)*(moyennePayoffCarre-moyennePayoff*moyennePayoff);

  ic = 1.96*sqrt(ksiCarreM/nbSamples_)*2;

  prix = exp(-mod_->r_*(opt_->T_-t))*moyennePayoff;

  pnl_mat_free(&pathCourant);
}

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta) {
  pnl_vect_resize(delta, opt_->size_);
  PnlMat *path = pnl_mat_create(opt_->nbTimeSteps_+1, opt_->size_);
  PnlMat *shift_path_plus = pnl_mat_new();
  PnlMat *shift_path_minus = pnl_mat_new();

  double timestep = opt_->T_/opt_->nbTimeSteps_;
  double Diff = 0;
  PnlVect *vectDiff = pnl_vect_create(opt_->size_);

  for (int i = 0; i < nbSamples_; i++){
    mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
    for (int d = 0; d<opt_->size_; d++) {
      mod_->shiftAsset(shift_path_plus, path, d, fdStep_ , t, timestep);
      mod_->shiftAsset(shift_path_minus, path, d, -fdStep_, t, timestep);
      Diff = opt_->payoff(shift_path_plus) - opt_->payoff(shift_path_minus);
      pnl_vect_set(vectDiff,d,Diff);

    }
    pnl_vect_plus_vect(delta, vectDiff);

  }


  double scal = exp(-mod_->r_*(opt_->T_-t))/(2*nbSamples_*fdStep_);
  pnl_vect_mult_scalar(delta, scal);

  PnlVect* s_t = pnl_vect_new();
  pnl_mat_get_row(s_t, past , past->m-1);
  pnl_vect_div_vect_term(delta,s_t); // Division par s_t


  pnl_vect_free(&s_t);
  pnl_vect_free(&vectDiff);

  pnl_mat_free(&path);
  pnl_mat_free(&shift_path_plus);
  pnl_mat_free(&shift_path_minus);
}

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic_delta) {
  pnl_vect_resize(delta, opt_->size_);
  pnl_vect_resize(ic_delta, opt_->size_);
  PnlMat *path = pnl_mat_create(opt_->nbTimeSteps_+1, opt_->size_);
  PnlMat *shift_path_plus = pnl_mat_new();
  PnlMat *shift_path_minus = pnl_mat_new();

  PnlVect *delta_carre = pnl_vect_create_from_scalar(opt_->size_,0);
  PnlVect *delta_2 = pnl_vect_create_from_scalar(opt_->size_,0);

  double timestep = opt_->T_/opt_->nbTimeSteps_;
  double Diff = 0;
  PnlVect *vectDiff = pnl_vect_create_from_scalar(opt_->size_,0);
  PnlVect *vectDiffCarre = pnl_vect_create_from_scalar(opt_->size_,0);

  for (int i = 0; i < nbSamples_; i++){
    mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
    for (int d = 0; d<opt_->size_; d++) {
      mod_->shiftAsset(shift_path_plus, path, d, fdStep_ , t, timestep);
      mod_->shiftAsset(shift_path_minus, path, d, -fdStep_, t, timestep);
      Diff = opt_->payoff(shift_path_plus) - opt_->payoff(shift_path_minus);
      pnl_vect_set(vectDiff, d, Diff);

      pnl_vect_set(vectDiffCarre, d, Diff*Diff);
    }
    pnl_vect_plus_vect(delta, vectDiff);

    pnl_vect_plus_vect(delta_carre, vectDiffCarre); // moyenne des carres
  }
  double exp_ = exp(-mod_->r_*(opt_->T_-t));
  // calcul delta
  double denominateur = 2*fdStep_;
  int nbRowsPast = past->m;
  PnlVect* s_t = pnl_vect_new();
  pnl_mat_get_row(s_t, past , nbRowsPast-1);

  pnl_vect_div_scalar(delta, denominateur);
  pnl_vect_div_scalar(delta, nbSamples_);
  pnl_vect_div_vect_term(delta,s_t); // Division par s_t
  //pause delta
  // calcul ic_delta
  pnl_vect_div_scalar(delta_carre, denominateur*denominateur);
  pnl_vect_div_vect_term(delta_carre,s_t);
  pnl_vect_div_vect_term(delta_carre,s_t);
  pnl_vect_div_scalar(delta_carre, nbSamples_);

  pnl_vect_plus_vect(delta_2,delta);
  pnl_vect_mult_vect_term(delta_2, delta);

  pnl_vect_plus_vect(ic_delta, delta_carre);
  pnl_vect_minus_vect(ic_delta, delta_2);

  pnl_vect_map_inplace(ic_delta, sqrt);
  pnl_vect_mult_scalar(ic_delta,2*1.96*exp_);

  //fin delta
  pnl_vect_mult_scalar(delta,exp_);


  pnl_vect_free(&s_t);
  pnl_vect_free(&vectDiff);
  pnl_vect_free(&vectDiffCarre);
  pnl_vect_free(&delta_2);
  pnl_vect_free(&delta_carre);
  pnl_mat_free(&path);
  pnl_mat_free(&shift_path_plus);
  pnl_mat_free(&shift_path_minus);
}