#include "BlackScholesModel.hpp"

using namespace std;


BlackScholesModel::BlackScholesModel()
{
	size_ = 0;
	r_= 0;
  dividend_= pnl_vect_new();
	rho_ = pnl_mat_new();
	sigma_ = pnl_vect_new();
	spot_ = pnl_vect_new();
	chol_ = pnl_mat_new();
	trend_ = pnl_vect_new();
}


BlackScholesModel::BlackScholesModel(const BlackScholesModel &BSM){
  size_ = BSM.size_;
  r_=  BSM.r_;
  dividend_=  pnl_vect_copy(BSM.dividend_);
  rho_ =  pnl_mat_copy(BSM.rho_);
  sigma_ =  pnl_vect_copy(BSM.sigma_);
  spot_ =  pnl_vect_copy(BSM.spot_);
  chol_ =  pnl_mat_copy(BSM.chol_);
  trend_ = pnl_vect_copy(BSM.trend_);
}

BlackScholesModel& BlackScholesModel::operator = (const BlackScholesModel &BSM)
{
	size_ = BSM.size_;
  r_=  BSM.r_;
  dividend_=  BSM.dividend_;
	rho_ = BSM.rho_;
	sigma_ = BSM.sigma_;
	spot_ = BSM.spot_;
	chol_ = BSM.chol_;
  trend_ = BSM.trend_;
	return *this;
}

BlackScholesModel::~BlackScholesModel() {
	  pnl_vect_free(&dividend_);
	  pnl_mat_free(&rho_);
    pnl_vect_free(&sigma_);
    pnl_vect_free(&spot_);
    pnl_mat_free(&chol_);
    pnl_vect_free(&trend_);
}

BlackScholesModel::BlackScholesModel(int size, double r , double rho, PnlVect *sigma, PnlVect *spot)
{
  size_ = size;
  r_ = r;
  dividend_ = pnl_vect_create_from_scalar(size,0.0);
  rho_ = pnl_mat_create_from_scalar(size,size,rho);
  for (int d = 0; d < size ; d++){
    pnl_mat_set(rho_, d, d, 1);
   }
  sigma_ = pnl_vect_copy(sigma);
  spot_ = pnl_vect_copy(spot);
  // la factorisée de Cholesky
  chol_ = pnl_mat_copy(rho_);
  int defPos = pnl_mat_chol(chol_);
  if (defPos == FAIL) {
    throw invalid_argument("la matrice de correlation n'est pas définie positive");
  }
  trend_ = pnl_vect_create_from_scalar(size,r);
}

BlackScholesModel::BlackScholesModel(int size, double r , double rho, PnlVect *sigma, PnlVect *spot, PnlVect *trend)
{
  size_ = size;
  r_ = r;
  dividend_ = pnl_vect_create_from_scalar(size,0.0);
  rho_ = pnl_mat_create_from_scalar(size,size,rho);
  for (int d = 0; d < size ; d++){
    pnl_mat_set(rho_, d, d, 1);
   }
  sigma_ = pnl_vect_copy(sigma);
  spot_ = pnl_vect_copy(spot);
  // la factorisée de Cholesky
  chol_ = pnl_mat_copy(rho_);
  int defPos = pnl_mat_chol(chol_);
  if (defPos == FAIL) {
    throw invalid_argument("la matrice de correlation n'est pas définie positive");
  }
  trend_ = pnl_vect_copy(trend);
}

BlackScholesModel::BlackScholesModel(int size, double r , PnlMat *rho, PnlVect *sigma, PnlVect *spot)
{
  size_ = size;
  r_ = r;
  dividend_ = pnl_vect_create_from_scalar(size,0.0);
  rho_ = pnl_mat_copy(rho);
  sigma_ = pnl_vect_copy(sigma);
  spot_ = pnl_vect_copy(spot);
  // la factorisée de Cholesky
  chol_ = pnl_mat_copy(rho_);
  int defPos = pnl_mat_chol(chol_);
  if (defPos == FAIL) {
    throw invalid_argument("la matrice de correlation n'est pas définie positive");
  }
  trend_ = pnl_vect_create_from_scalar(size,r);
}

BlackScholesModel::BlackScholesModel(int size, double r , PnlMat *rho, PnlVect *sigma, PnlVect *spot, PnlVect *trend)
{
  size_ = size;
  r_ = r;
  dividend_ = pnl_vect_create_from_scalar(size, 0.0);
  rho_ = pnl_mat_copy(rho);
  sigma_ = pnl_vect_copy(sigma);
  spot_ = pnl_vect_copy(spot);
  // la factorisée de Cholesky
  chol_ = pnl_mat_copy(rho_);
  int defPos = pnl_mat_chol(chol_);
  if (defPos == FAIL) {
    throw invalid_argument("la matrice de correlation n'est pas définie positive");
  }
  trend_ = pnl_vect_copy(trend);
}



BlackScholesModel::BlackScholesModel(int size, double r, PnlMat *rho, PnlVect *sigma, PnlVect *spot, PnlVect *trend ,PnlVect *dividend)
{
  size_ = size;
  r_ = r;
  dividend_ = pnl_vect_copy(dividend);
  rho_ = pnl_mat_copy(rho);
  sigma_ = pnl_vect_copy(sigma);
  spot_ = pnl_vect_copy(spot);
  // la factorisée de Cholesky
  chol_ = pnl_mat_copy(rho_);
  int defPos = pnl_mat_chol(chol_);
  if (defPos == FAIL) {
    throw invalid_argument("la matrice de correlation n'est pas définie positive");
  }
  trend_ = pnl_vect_copy(trend);
}



void BlackScholesModel::simulateAsset(PnlMat *path, double timestep, int nbTimeSteps, RandomGen *rng, PnlVect *r){
  //assert(path->m == nbTimeSteps + 1);
  if(path->m != nbTimeSteps + 1){
    throw length_error("La matrice path est de mauvaise taille") ;
  }
  PnlVect* S_previous = pnl_vect_new();
  pnl_mat_get_row(S_previous, path, 0); // S_0
  // premierTerme = (r - sigma^²/2) * ( t_(i+1) - t_(i) )
  PnlVect* premierTerme = pnl_vect_copy(sigma_);
  pnl_vect_mult_vect_term(premierTerme, sigma_);
  pnl_vect_mult_scalar(premierTerme, -0.5);
  pnl_vect_plus_vect(premierTerme, r);
  pnl_vect_minus_vect(premierTerme, dividend_);
  pnl_vect_mult_scalar(premierTerme, timestep);
  // Mouvement Brownien standard à valeurs dans R^D
  PnlMat* MBS = pnl_mat_new();
  /////pnl_mat_rng_normal(MBS, nbTimeSteps, size_,rng);
  rng->mat_normal(MBS, nbTimeSteps, size_);
  PnlVect* V = pnl_vect_new();
  PnlVect *temp = pnl_vect_new();
  
  for (int t = 0; t < nbTimeSteps; t++) {
      //deuxièmeTerme = sigma * sqrt( t_(i+1) - t_(i) ) * choleskyFactor.MBS_t
      pnl_mat_get_row(V, MBS, t);

      pnl_mat_mult_vect_inplace(temp,chol_, V);
      pnl_vect_clone(V, temp);
      

      pnl_vect_mult_scalar(V, sqrt(timestep));
      pnl_vect_mult_vect_term(V, sigma_);
      // premierTerme + deuxièmeTerme
      pnl_vect_plus_vect(V, premierTerme);              // premierTerme + deuxièmeTerme
      pnl_vect_map_inplace(V, exp);                 // exp(            ...             )
      pnl_vect_mult_vect_term(V, S_previous); // S_t * ...
      pnl_mat_set_row(path, V , t + 1 );// S_(t+1)= ...
      pnl_vect_clone(S_previous, V); // S_t
  }
  pnl_vect_free(&temp);
  pnl_mat_free(&MBS);
  pnl_vect_free(&S_previous);
  pnl_vect_free(&V);
  pnl_vect_free(&premierTerme);
}

void BlackScholesModel::asset(PnlMat *path, double T, int nbTimeSteps, RandomGen *rng)
{
  if (!(path->m == nbTimeSteps+1 && path->n == size_)) {
    pnl_mat_resize(path, nbTimeSteps + 1, size_);
  }
  pnl_mat_set_row(path, spot_, 0);
  double timestep = T/nbTimeSteps;
  PnlVect* r = pnl_vect_create_from_scalar(size_,r_);
  simulateAsset(path, timestep, nbTimeSteps, rng, r);
  pnl_vect_free(&r);
}

void BlackScholesModel::asset(PnlMat *path, double t, double T, int nbTimeSteps, RandomGen *rng, const PnlMat *past)
{
  //assert(path->m == nbTimeSteps+1 && path->n == size_);
  if (!(path->m == nbTimeSteps+1 && path->n == size_)) {
    pnl_mat_resize(path, nbTimeSteps + 1, size_);
  }
  PnlVect* r = pnl_vect_create_from_scalar(size_,r_);
  double timestep = T/nbTimeSteps;
  int nbRowsPast = past->m;
  int nbTimeStepsResidual = (nbTimeSteps+1) - nbRowsPast;
  PnlMat* future = pnl_mat_create(nbTimeStepsResidual+1, size_);
  PnlVect* s_t = pnl_vect_new();
  pnl_mat_get_row(s_t, past, nbRowsPast-1);
  //assert(nbRowsPast == ceil(t/timestep)+1);
  if(nbRowsPast != ceil(t/timestep)+1){
    throw length_error("La matrice past ne correspond pas à l'instant t indiqué") ;
  }
  if (nbRowsPast-1 == t/timestep){
    pnl_mat_set_row(future, s_t, 0);
  }
  else{
    double delta = (nbRowsPast - 1)*timestep -t;
    PnlMat* temp = pnl_mat_create(2, size_);
    pnl_mat_set_row(temp, s_t, 0);
    simulateAsset(temp, delta, 1, rng, r);
    PnlVect* s_tS_delta = pnl_vect_new();
    pnl_mat_get_row(s_tS_delta, temp , 1);
    pnl_mat_set_row(future, s_tS_delta, 0);

    pnl_mat_free(&temp);
    pnl_vect_free(&s_tS_delta);
  }

  simulateAsset(future, timestep, nbTimeStepsResidual, rng, r);
  pnl_vect_free(&r);

  if (past->m == 1){
    pnl_mat_clone(path,future);
  }
  else{
    PnlMat* subPast = pnl_mat_new();
    pnl_mat_extract_subblock(subPast, past, 0, nbRowsPast-1, 0, size_);
    Tools::concatenationMatrice( path , subPast, future );
    pnl_mat_free(&subPast);
  }

  pnl_vect_free(&s_t);
  pnl_mat_free(&future);
}

void BlackScholesModel::shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep)
{
  pnl_mat_clone(shift_path, path);
  int rowToShift = ceil(t/timestep);
  while (rowToShift < shift_path->m)
  {
    pnl_mat_set(shift_path, rowToShift, d, pnl_mat_get(shift_path, rowToShift, d)*(1+h));
    rowToShift++;
  }
}

void BlackScholesModel::simul_market(PnlMat* market, double T, int H, RandomGen *rng)
{
  pnl_mat_resize(market, H+1, size_);
  pnl_mat_set_row(market, spot_, 0);
  double timestep = T/H;
  simulateAsset(market, timestep, H, rng, trend_);
}
