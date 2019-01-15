#include "MonteCarlo.hpp"
#include "Call.hpp"
#include "BlackScholesModel.hpp"
#include "Delta_DF.hpp"
#include <math.h>
using namespace std;
using namespace Computations;
double TrendtimeSteps;

/* Constructeur par défault */
MonteCarlo::MonteCarlo(){
	delta_ = new Delta_DF();
	mod_ = new BlackScholesModel();
	opt_ = new Call();
	rng_ = pnl_rng_create(PNL_RNG_MERSENNE);
    fdStep_ = 0;
    nbSamples_= 0;
 }

MonteCarlo::MonteCarlo(DeltaCompute *delta, AssetModel *mod,Option *opt,int nbSamples, PnlRng *rng, double fdStep){
   delta_ = delta;
   mod_ = mod;
   opt_ = opt;
   rng_ = pnl_rng_copy(rng);
   fdStep_= fdStep;
   nbSamples_ = nbSamples;
   TrendtimeSteps = opt_->T_ / opt_->nbTimeSteps_;
 }

/* Destrcuteur de MonteCarlo */
MonteCarlo::~MonteCarlo(){
   delta_->~DeltaCompute();
   opt_->~Option();
   mod_->~AssetModel();
   pnl_rng_free(&rng_);
 }

/** Calcul du prix */
void MonteCarlo::price(double &prix, double &ic){
   PnlMat *path = pnl_mat_create ( opt_->nbTimeSteps_+1,opt_->size_);
   PnlVect *vectActualisation = pnl_vect_new();
   double results = 0;
   double esp_carre = 0;
   for(int i = 0;i < nbSamples_;i++){
     mod_->asset(path, opt_->T_,opt_->nbTimeSteps_, rng_);
	 // Le trend a été update dans asset
	 vectActualisation = pnl_vect_copy(mod_->trend_); // Récupération du vecteur de trend
	 pnl_vect_map_inplace(vectActualisation, &Trenddrift); // le vecteur de trend * -(T/N) ( on est dans le cas 0)
     double payOff = opt_->payoff(path);
	 payOff *= exp(pnl_vect_sum(vectActualisation)); // intégrale sous forme de somme discrète
     results += payOff;
     esp_carre += payOff*payOff;
   }
   results = (results/nbSamples_);
   esp_carre = (esp_carre/nbSamples_);
   double esp = results;
   prix = results;

   double estim_carre = (esp_carre-esp*esp);
   ic = 2*1.96*sqrt(estim_carre/nbSamples_);
   pnl_mat_free(&path);
   pnl_vect_free(&vectActualisation);
 }

 /** Calcul du Delta */
 void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic){
	 delta_->delta(past, t, delta, ic, mod_, opt_, rng_, nbSamples_);
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
  int nbRowsPast = past->m;
  PnlVect* s_t = pnl_vect_new();
  pnl_mat_get_row(s_t, past , nbRowsPast-1);
  pnl_vect_div_vect_term(delta,s_t); // Division par s_t


  pnl_vect_free(&s_t);
  pnl_vect_free(&vectDiff);

  pnl_mat_free(&path);
  pnl_mat_free(&shift_path_plus);
  pnl_mat_free(&shift_path_minus);
  pnl_vect_free(&vectDiff);
}



//fonction qui permet de calculer le prix d'une option à n'importe quelle instant t

 void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &ic){
   PnlMat *path = pnl_mat_create ( opt_->nbTimeSteps_+1,opt_->size_);
   PnlVect *vectActualisation = pnl_vect_new(); // Ce vecteur contiendra les taux d'intérêt multiplié par le temps
   double results = 0;
   double esp_carre=0;
   for(int i=0;i<nbSamples_;i++){
     mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
	 vectActualisation = pnl_vect_copy(mod_->trend_); // on récupère le trend qui a été mise à jour dans asset juste après le calcul des taux d'intérêt
	 pnl_vect_map_inplace(vectActualisation, &Trenddrift); // on multiplie tous les éléments du trend par T/n 
	 // ComputeFirst time step retourne directement ti+i - t
	 pnl_vect_set(vectActualisation, 0, pnl_vect_get(vectActualisation, 0) * (computeFirstTimeSteps(TrendtimeSteps, t) / TrendtimeSteps)); // on divise le premier élément par T/n pour le multiplier à Ti+1 - t
     double payOff = opt_->payoff(path);
	 payOff *= exp(pnl_vect_sum(vectActualisation));
     results += payOff;
     esp_carre += payOff*payOff;
   }
   results = (results/nbSamples_);
   esp_carre = (esp_carre/nbSamples_);
   double esp = results;

   prix = results;

   //calcul du ic
   double estim_carre = (esp_carre-esp*esp);
   ic = 2*1.96*sqrt(estim_carre/nbSamples_);
   pnl_mat_free(&path);
   pnl_vect_free(&vectActualisation);
 }

void MonteCarlo::Profit_and_loss(const PnlMat* marche,double &PL ,const int H ){

  PnlMat *past =  pnl_mat_create_from_scalar(1,opt_->size_,0);
  PnlVect *extracted_vec = pnl_vect_create(opt_->size_);
  pnl_mat_get_row (extracted_vec, marche, 0);
  pnl_mat_set_row (past, extracted_vec, 0);


  /* Calcul de P&L*/
  double p_0 = 0;
  double ic = 0;
  this->price(p_0,ic);
  PnlVect *icdelta = pnl_vect_create(mod_->size_);
  PnlVect *delta_0 = pnl_vect_create(mod_->size_);
  this->delta(past,0.0,delta_0,icdelta);
  PnlVect *S_0 = pnl_vect_create(mod_->size_);
  pnl_mat_get_row(S_0,marche,0);
  PnlVect *V = pnl_vect_create(H+1);
  pnl_vect_set (V, 0, p_0 - pnl_vect_scalar_prod(delta_0,S_0));

  PnlVect *delta_pres = pnl_vect_copy (delta_0);
  PnlVect *delta_act = pnl_vect_create(mod_->size_);
  PnlVect *S_i = pnl_vect_create(mod_->size_);
  PnlVect *vect_diff = pnl_vect_create_from_zero(mod_->size_);
  double v_i = 0;
  int nb_rows = 1;
  int count = 1 ;
  for(int i=1;i<=H;i++){
    if(i*opt_->T_/H == count *opt_->T_/opt_->nbTimeSteps_ ){
      count +=1;
      nb_rows+=1;
      pnl_mat_resize (past,nb_rows, mod_->size_);
    }
    pnl_mat_get_row(extracted_vec,marche,i);
    pnl_mat_set_row(past,extracted_vec,nb_rows-1);
    if(i!=H){
        this->delta(past,i*opt_->T_/H,delta_act,icdelta);
    }
    vect_diff = pnl_vect_copy(delta_act);
    pnl_vect_minus_vect (vect_diff, delta_pres);
    pnl_mat_get_row(S_i,marche,i);
	// TO DO BECAUSE I DON't UNDERSTAND 
	/*
	
	v_i =pnl_vect_get (V,i-1)*exp(r_*opt_->T_/H) - pnl_vect_scalar_prod(vect_diff,S_i); PREVIOUS

	*/
	// and now modified NOT GOOD but I have to send a version. i PUT 0.05 INSTEAD OF r_
    v_i =pnl_vect_get (V,i-1)*exp(0.05*opt_->T_/H) - pnl_vect_scalar_prod(vect_diff,S_i);
    pnl_vect_set (V, i, v_i);
    delta_pres =pnl_vect_copy(delta_act) ;

  }
  double v_h = pnl_vect_get (V,H);
  pnl_mat_get_row(S_i,marche,H);
  PL = v_h + pnl_vect_scalar_prod(delta_pres,S_i) - opt_->payoff(past) ;



  pnl_mat_free(&past);
  pnl_vect_free(&extracted_vec);
  pnl_vect_free(&delta_0);
  pnl_vect_free(&S_0);
  pnl_vect_free(&V);
  pnl_vect_free(&delta_pres);
  pnl_vect_free(&S_i);
  pnl_vect_free(&vect_diff);
  pnl_vect_free(&delta_act);


}



double computeFirstTimeSteps(double timeSteps, double t) {
	int k = 0;
	while (t > k * timeSteps) {
		k = k + 1;
	}
	return (k * timeSteps) - t;
}

double Trenddrift(double TrendElement) {
	return -TrendElement * TrendtimeSteps;
}