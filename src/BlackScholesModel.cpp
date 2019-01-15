#include "BlackScholesModel.hpp"
#include <math.h>
#include <iostream>
#include <stdexcept>
using namespace std;
using namespace Computations;

BlackScholesModel::BlackScholesModel()
{
	trend_ = pnl_vect_new();
}


BlackScholesModel::BlackScholesModel(const BlackScholesModel &BlackScholesModelACopier){
  size_ = BlackScholesModelACopier.size_;
  r_=  BlackScholesModelACopier.r_;
  rho_ =  BlackScholesModelACopier.rho_;
  sigma_ =  pnl_vect_copy(BlackScholesModelACopier.sigma_);
  spot_ =  pnl_vect_copy(BlackScholesModelACopier.spot_);
  choleskyFactor =  pnl_mat_copy(BlackScholesModelACopier.choleskyFactor);
  trend_ = pnl_vect_copy(BlackScholesModelACopier.trend_);
}

BlackScholesModel::~BlackScholesModel() {
    pnl_vect_free(&sigma_);
    pnl_vect_free(&spot_);
    pnl_mat_free(&choleskyFactor);
    pnl_vect_free(&trend_);
}


BlackScholesModel::BlackScholesModel(int size, double r , double rho, PnlVect *sigma, PnlVect *spot)
{
  size_ = size;
  r_ = r;
  if (rho > 1 || (size!=1 && rho < (-1/(size-1)))) {
    throw invalid_argument("la correlation n'est pas dans le bon intervalle");
  }
  rho_ = rho;
  sigma_ = pnl_vect_copy(sigma);
  spot_ = pnl_vect_copy(spot);


  // la factorisée de Cholesky
  choleskyFactor = pnl_mat_create_from_scalar(size_, size_, rho_);
  for (int d = 0; d < size_ ; d++){
    pnl_mat_set(choleskyFactor, d, d, 1);
  }
  pnl_mat_chol(choleskyFactor);
  trend_ = pnl_vect_create(size);
}


BlackScholesModel::BlackScholesModel(int size, double r , double rho, PnlVect *sigma, PnlVect *spot, PnlVect *trend)
{
  size_ = size;
  r_ = r;
  if (rho > 1 || (size!=1 && rho < (-1/(size-1)))) {
    throw invalid_argument("la correlation n'est pas dans le bon intervalle");
  }
  rho_ = rho;
  sigma_ = pnl_vect_copy(sigma);
  spot_ = pnl_vect_copy(spot);
  trend_ = pnl_vect_copy(trend);

  // la factorisée de Cholesky
  choleskyFactor = pnl_mat_create_from_scalar(size_, size_, rho_);
  for (int d = 0; d < size_ ; d++){
    pnl_mat_set(choleskyFactor, d, d, 1);
  }
  pnl_mat_chol(choleskyFactor);

}


BlackScholesModel& BlackScholesModel::operator = (const BlackScholesModel &BSMM) //le const c'est pour traduire le fait que cet opérateur ne modifie pas le Dvector
{
	size_ = BSMM.size_;
	interest_ = BSMM.interest_;
	corr_ = BSMM.corr_;
	sigma_ = BSMM.sigma_;
	spot_ = BSMM.spot_;
	trend_ = BSMM.trend_;
	chol_ = BSMM.chol_;
	return *this;
}



void BlackScholesModel::concatenationMatrice(PnlMat* res, const PnlMat *mat1, const PnlMat *mat2){
  assert(mat1->n == mat2->n && mat2->n == res->n);
  assert(res->m == mat1->m + mat2->m);
  int nbRows= res->m;
  int nbColumns = res->n;
  PnlVect* currentRow = pnl_vect_create(nbColumns);
  for (int i = 0; i < mat1->m; i++) {
    pnl_mat_get_row(currentRow, mat1, i);
    pnl_mat_set_row(res, currentRow, i);
  }
  for (int i = mat1->m; i < nbRows; i++) {
    pnl_mat_get_row(currentRow, mat2, i - mat1->m);
    pnl_mat_set_row(res, currentRow, i);
  }

  pnl_vect_free(&currentRow);
}



void BlackScholesModel::simulateAsset(PnlMat *path, double timestep, int nbTimeSteps, RandomGen *rng, PnlVect* r){
  PnlVect* S_previous = pnl_vect_new();
  pnl_mat_get_row(S_previous, path, 0); // S_0
  // premierTerme = (r - sigma^²/2) * ( t_(i+1) - t_(i) )
  PnlVect* premierTerme = pnl_vect_copy(sigma_);
  pnl_vect_mult_vect_term(premierTerme, sigma_);
  pnl_vect_mult_scalar(premierTerme, -0.5);
  pnl_vect_plus_vect(premierTerme, r);
  pnl_vect_mult_scalar(premierTerme, timestep);
  // Mouvement Brownien standard à valeurs dans R^D
  PnlMat* MBS = pnl_mat_new();
  rng->mat_normal(MBS, nbTimeSteps, size_);
  PnlVect* V = pnl_vect_new();

  for (int t = 0; t < nbTimeSteps; t++) {
      //deuxièmeTerme = sigma * sqrt( t_(i+1) - t_(i) ) * choleskyFactor.MBS_t
      pnl_mat_get_row(V, MBS, t);

      PnlVect *temp = pnl_mat_mult_vect(choleskyFactor, V);
      pnl_vect_clone(V, temp);
      pnl_vect_free(&temp);

      pnl_vect_mult_scalar(V, sqrt(timestep));
      pnl_vect_mult_vect_term(V, sigma_);
      // premierTerme + deuxièmeTerme
      pnl_vect_plus_vect(V, premierTerme);              // premierTerme + deuxièmeTerme
      pnl_vect_map_inplace(V, exp);                 // exp(            ...             )
      pnl_vect_mult_vect_term(V, S_previous); // S_t * ...
      pnl_mat_set_row(path, V , t + 1 );// S_(t+1)= ...
      pnl_vect_clone(S_previous, V); // S_t
  }
  pnl_mat_free(&MBS);
  pnl_vect_free(&S_previous);
  pnl_vect_free(&V);
  pnl_vect_free(&premierTerme);
}

void BlackScholesModel::asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng) {
	assert(path->m == nbTimeSteps+1 && path->n == size_);
	pnl_mat_set_row(path, spot_, 0);
	double timestep = T/nbTimeSteps;
	PnlVect* r = pnl_vect_create_from_scalar(size_, r_);
	simulateAsset(path, timestep, nbTimeSteps, rng, r);
	pnl_vect_free(&r);

}



void BlackScholesModel::asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past) {
	assert(path->m == nbTimeSteps+1 && path->n == size_);
	double timestep = T/nbTimeSteps;
	int nbRowsPast = past->m;
	int nbTimeStepsResidual = (nbTimeSteps+1) - nbRowsPast;
	PnlMat* future = pnl_mat_create(nbTimeStepsResidual+1, size_);
	PnlVect* s_t = pnl_vect_new();
	pnl_mat_get_row(s_t, past, nbRowsPast-1);
	assert(nbRowsPast = ceil(t/timestep)+1);
	PnlVect* r = pnl_vect_create_from_scalar(size_, r_);
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

	if (past->m == 1){
		pnl_mat_clone(path,future);
	}
	else{
		PnlMat* subPast = pnl_mat_new();
		pnl_mat_extract_subblock(subPast,past,0,nbRowsPast-1,0,size_);
		concatenationMatrice( path , subPast, future );
		pnl_mat_free(&subPast);
	}

	pnl_vect_free(&s_t);
	pnl_mat_free(&future);
	pnl_vect_free(&r);
}




/**
* Calcule une trajectoire du sous-jacent connaissant le
* passé jusqu' à la date t
*
* @param[out] path  contient une trajectoire du sous-jacent
* donnée jusqu'à l'instant t par la matrice past
* @param[in] t date jusqu'à laquelle on connait la trajectoire.
* t n'est pas forcément une date de discrétisation
* @param[in] nbTimeSteps nombre de pas de constatation
* @param[in] T date jusqu'à laquelle on simule la trajectoire
* @param[in] past trajectoire réalisée jusqu'a la date t
*/
void BlackScholesModel::asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past, const PnlMat *pastInterest) {
	if ((path->n != past->n) || (t > T)) {
		cout << "valeur de path->n : " << path->n << endl;
		cout << "valeur de past->n : " << past->n << endl;
		cout << "valeur de t : " << t << endl;
		cout << "valeur de T : " << T << endl;
		throw length_error("invalide taille");
	}

	/** Initialisation des vecteurs de calcul */
	PnlVect *G = pnl_vect_new();
	PnlVect *V = pnl_vect_create(size_);
	PnlVect *W = pnl_vect_create(size_);

	double timestep = T / (nbTimeSteps);
	double sqrtTimestep = sqrt(timestep);
	double sigmaJ = 0.0;
	PnlMat *pathInterest = pnl_mat_new();
	interest_->interest(pathInterest, t, T, nbTimeSteps, rng, pastInterest);

	/** Initialisation des paramètres */
	double timeSpend = 0;
	int counter = 0;

	/** Remplissage de la matrice path par la matrice past jusqu'à t*/
	while ((t >= timeSpend)/*&&( timeSpend < T)*/) {
		pnl_mat_get_row(V, past, counter);
		pnl_mat_set_row(path, V, counter);
		timeSpend += timestep;
		counter += 1;
	}

	// On récupère St
	pnl_mat_get_row(W, past, past->m - 1);

	/** Calcul de la matrice des trajectoires des actifs */
	for (int i = counter; i < path->m; i++) {
		pnl_vect_rng_normal(G, size_, rng);
		for (int j = 0; j < path->n; j++) {
			pnl_mat_get_row(V, chol_, j);
			sigmaJ = pnl_vect_get(sigma_, j);
			if (i == counter) {
				// on place St en première ligne
				pnl_mat_set(path, i, j, pnl_vect_get(W, j) * exp((pnl_mat_get(pathInterest, i, j) - (sigmaJ*sigmaJ) / 2) * (timeSpend - t) + sigmaJ * sqrt(timeSpend - t) * pnl_vect_scalar_prod(V, G)));
			}
			else {
				if (i != path->m) {
					pnl_mat_set(path, i, j, pnl_mat_get(path, i - 1, j) * exp((pnl_mat_get(pathInterest, i, j) - (sigmaJ*sigmaJ) / 2) * timestep + sigmaJ * sqrtTimestep * pnl_vect_scalar_prod(V, G)));
				}
			}
		}
	}

	/** Libération de la mémoire */
	pnl_vect_free(&W);
	pnl_vect_free(&G);
	pnl_vect_free(&V);

}

