#include "MonteCarlo.hpp"
#include "Asian.hpp"
#include "BlackScholesModel.hpp"
#include <math.h>
using namespace std;
using namespace Computations;

/* Constructeur par défault */
MonteCarlo::MonteCarlo(){
   mod_ = new BlackScholesModel();
   opt_ = new Asian();
   rng_ = pnl_rng_create(PNL_RNG_MERSENNE);
   r_ = 0;
   fdStep_ = 0;
   nbSamples_= 0;
 }

MonteCarlo::MonteCarlo(AssetModel *mod,Option *opt,int nbSamples, PnlRng *rng, double fdStep, double r){
   mod_ = mod;
   opt_ = opt;
   rng_ = pnl_rng_copy(rng);
   fdStep_= fdStep;
   nbSamples_ = nbSamples;
   r_ = r;
 }

/* Destrcuteur de MonteCarlo */
MonteCarlo::~MonteCarlo(){
   opt_->~Option();
   mod_->~AssetModel();
   pnl_rng_free(&rng_);
 }

/** Calcul du prix */
void MonteCarlo::price(double &prix, double &ic){
   PnlMat *path = pnl_mat_create ( opt_->nbTimeSteps_+1,opt_->size_);
   double results = 0;
   double esp_carre = 0;
   for(int i = 0;i < nbSamples_;i++){
     mod_->asset(path, opt_->T_,opt_->nbTimeSteps_, rng_);
     double payOff = opt_->payoff(path);
     results += payOff;
     esp_carre += payOff*payOff;
   }
   results = (results/nbSamples_);
   esp_carre = (esp_carre/nbSamples_);
   double esp = results;
   prix = exp(-r_*(opt_->T_))*results;

   double estim_carre = exp(-2*r_*(opt_->T_))*(esp_carre-esp*esp);
   ic = 2*1.96*sqrt(estim_carre/nbSamples_);
   pnl_mat_free(&path);
 }

 /** Calcul du Delta */
 void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic){

   //initialisation de la matrice path.
   PnlMat *path = pnl_mat_create(opt_->nbTimeSteps_+1,opt_->size_);
   double timestep = opt_->T_/opt_->nbTimeSteps_;

   //pas de la méthode.
   double h = fdStep_;

   //initialisation des matrices shift_path pour +h(et shift_path_inverse pour -h)
   PnlMat *shift_path = pnl_mat_create_from_scalar ( opt_->nbTimeSteps_+1,opt_->size_,0);
   PnlMat *shift_path_inverse = pnl_mat_create_from_scalar ( opt_->nbTimeSteps_+1,opt_->size_,0);

  //Delta calculer pour chaque actif
  double Delta = 0;

  //vecteur des valeurs des actifs à l'instant t.
  PnlVect *V = pnl_vect_create(mod_->size_);
  pnl_mat_get_row(V, past, past->m - 1);

   double param_actualisation = 0;
   for(int d = 1; d < path->n + 1; d++){
     double var = 0;
     Delta = 0;
     double diff = 0.0;
     param_actualisation = (exp(-r_*(opt_->T_ - t)))/(nbSamples_*2*h);
     for (size_t i__ = 0; i__ <nbSamples_; i__++) {
       //chargement de la matrice path par la trajectoire.
       mod_->asset(path,t,opt_->T_,opt_->nbTimeSteps_,rng_,past);
       //pnl_mat_print(path);
       //avoir la matrice shifté pour l'actif d
       mod_->shiftAsset(shift_path, path, d, h, t, timestep);
       //pnl_mat_print(shift_path);

       mod_->shiftAsset(shift_path_inverse, path, d, -h, t, timestep);
       //pnl_mat_print(shift_path_inverse);

      //ajout du resultat du payoff à l'esperance empirique.
      diff = opt_->payoff(shift_path) - opt_->payoff(shift_path_inverse);
      Delta += diff;
      var += diff*diff;
     }

    param_actualisation /= pnl_vect_get(V,d-1);
    Delta *= param_actualisation;
    var *= (param_actualisation*(nbSamples_))*(param_actualisation*(nbSamples_));
    var /= nbSamples_;
    //stockage de Delta en delta à la position d
    pnl_vect_set(delta,d -1,Delta);
    pnl_vect_set(ic,d -1, 2*1.96*sqrt((var - Delta*Delta)/nbSamples_));
   }

 }



//fonction qui permet de calculer le prix d'une option à n'importe quelle instant t

 void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &ic){
   PnlMat *path = pnl_mat_create ( opt_->nbTimeSteps_+1,opt_->size_);
   double results = 0;
   double esp_carre=0;
   for(int i=0;i<nbSamples_;i++){
     mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
     double payOff = opt_->payoff(path);
     results += payOff;
     esp_carre += payOff*payOff;
   }
   results = (results/nbSamples_);
   esp_carre = (esp_carre/nbSamples_);
   double esp = results;
   prix = exp(-r_*((opt_->T_)-t))*results;

   //calcul du ic
   double estim_carre = exp(-2*r_*((opt_->T_)-t))*(esp_carre-esp*esp);
   ic = 2*1.96*sqrt(estim_carre/nbSamples_);
   pnl_mat_free(&path);

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
    v_i =pnl_vect_get (V,i-1)*exp(r_*opt_->T_/H) - pnl_vect_scalar_prod(vect_diff,S_i);
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
 /*int main(int argc, char **argv)
 {
   PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
   double T, r, strike, rho;
   PnlVect *spot, *sigma, *divid;
   string type;
   int size;
   size_t n_samples;
   char *infile = argv[1];{t 0 , . . . , t N }
   int M = 1E2; // à calculer selon l'intervalle de confiance
   PnlMat *path = pnl_mat_create(M,size);
   Param *P = new Parser(infile);

   P->extract("option type", type);
   P->extract("maturity", T);
   P->extract("option size", size);
   P->extract("spot", spot, size);
   P->extract("volatility", sigma, size);
   P->extract("interest rate", r);
   P->extract("correlation", rho);
   if (P->extract("dividend rate", divid, size, true) == false)
   {
       divid = pnl_vect_create_from_zero(size);
   }
   P->extract("strike", strike);
   P->extract("sample number", n_samples);

   BlackScholesModel bsm = BlackScholesModel(size, r, rho, sigma, spot);
   bsm.asset(path, 3.0, M, rng);
   pnl_mat_print(path);
   //Option option = Option(, size);
   //MonteCarlo mc = MonteCarlo();

   delete P;
   exit(0);
}*/
