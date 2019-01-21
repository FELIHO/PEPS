#include "BlackScholesMertonModel.hpp"

using namespace std;
using namespace Computations;

/**
Constructeur par d�faut
*/
BlackScholesMertonModel::BlackScholesMertonModel() : BlackScholesModel()
{	
	dividend_ = pnl_vect_new();
}


/**
Constructeur complet
*/
BlackScholesMertonModel::BlackScholesMertonModel(int size, PnlVect *r, PnlMat *rho, PnlVect *sigma, PnlVect *spot, PnlVect *dividend) : BlackScholesModel(size, r, rho, sigma, spot)
{
	dividend_ = pnl_vect_copy(dividend);
}



/**
Destructeur
*/

BlackScholesMertonModel::~BlackScholesMertonModel()
{	
	pnl_vect_free(&r_);
	pnl_mat_free(&rho_);
	pnl_vect_free(&sigma_);
	pnl_vect_free(&spot_);
	pnl_mat_free(&chol_);
	pnl_mat_free(&trend_);
	pnl_vect_free(&dividend_);
}


/** Methode d'affectation d'un BlackScholesMertonModel
* @param[in] une image de la classe BlackScholesMertonModel � affecter.
* @param[out] la m�me r�f�rence BlackScholesMertonModel avec les m�mes param�tres que l'entr�e
*/
BlackScholesMertonModel& BlackScholesMertonModel::operator = (const BlackScholesMertonModel &BSMM) //le const c'est pour traduire le fait que cet op�rateur ne modifie pas le Dvector
{
	size_ = BSMM.size_;
	rho_ = BSMM.rho_;
	sigma_ = BSMM.sigma_;
	spot_ = BSMM.spot_;
	trend_ = BSMM.trend_;
	chol_ = BSMM.chol_;
	dividend_ = BSMM.dividend_;
	return *this;
}

/**
* G�n�re une trajectoire du mod�le et la stocke dans path
*
* @param[out] path contient une trajectoire du mod�le.
* C'est une matrice de taille (nbTimeSteps+1) x d
* @param[in] T  maturit�
* @param[in] nbTimeSteps nombre de dates de constatation
*/
void BlackScholesMertonModel::asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng) {

	/** Initialisation des vecteurs de calcul */
	PnlVect *G = pnl_vect_new();
	PnlVect *V = pnl_vect_new();
	double timestep = T / nbTimeSteps;
	double sqrtTimestep = sqrt(timestep);
	double sigmaJ = 0.0;
	PnlMat *pathInterest = pnl_mat_new();
	//interest_->interest(pathInterest, T, nbTimeSteps, rng);
	//updateTrend(pathInterest);

	// Premi�re ligne
	pnl_mat_set_row(path, spot_, 0);

	/** Calcul de la matrice des trajectoires des actifs */
	for (int i = 0; i < path->m - 1; i++) {
		pnl_vect_rng_normal(G, size_, rng);
		for (int j = 0; j < path->n; j++) {
			pnl_mat_get_row(V, chol_, j);
			sigmaJ = pnl_vect_get(sigma_, j);
			pnl_mat_set(path, i + 1, j, pnl_mat_get(path, i, j) * exp((pnl_mat_get(pathInterest, i, j) - pnl_vect_get(dividend_, j) - (sigmaJ*sigmaJ) / 2) * timestep + sigmaJ * sqrtTimestep * pnl_vect_scalar_prod(V, G)));
		}
	}

	/** Lib�ration de la m�moire */
	pnl_vect_free(&G);
	pnl_vect_free(&V);

}


/**
* Calcule une trajectoire du sous-jacent connaissant le
* pass� jusqu' � la date t
*
* @param[out] path  contient une trajectoire du sous-jacent
* donn�e jusqu'� l'instant t par la matrice past
* @param[in] t date jusqu'� laquelle on connait la trajectoire.
* t n'est pas forc�ment une date de discr�tisation
* @param[in] nbTimeSteps nombre de pas de constatation
* @param[in] T date jusqu'� laquelle on simule la trajectoire
* @param[in] past trajectoire r�alis�e jusqu'a la date t
*/
void BlackScholesMertonModel::asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past) {
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
	//interest_->interest(pathInterest, T, nbTimeSteps, rng);
	//updateTrend(pathInterest);

	/** Initialisation des param�tres */
	double timeSpend = 0;
	int counter = 0;

	/** Remplissage de la matrice path par la matrice past jusqu'� t*/
	while ((t >= timeSpend)/*&&( timeSpend < T)*/) {
		pnl_mat_get_row(V, past, counter);
		pnl_mat_set_row(path, V, counter);
		timeSpend += timestep;
		counter += 1;
	}

	// On r�cup�re St
	pnl_mat_get_row(W, past, past->m - 1);

	/** Calcul de la matrice des trajectoires des actifs */
	for (int i = counter; i < path->m; i++) {
		pnl_vect_rng_normal(G, size_, rng);
		for (int j = 0; j < path->n; j++) {
			pnl_mat_get_row(V, chol_, j);
			sigmaJ = pnl_vect_get(sigma_, j);
			if (i == counter) {
				// on place St en premi�re ligne
				pnl_mat_set(path, i, j, pnl_vect_get(W, j) * exp((pnl_mat_get(pathInterest, i, j) - pnl_vect_get(dividend_, j) - (sigmaJ*sigmaJ) / 2) * (timeSpend - t) + sigmaJ * sqrt(timeSpend - t) * pnl_vect_scalar_prod(V, G)));
			}
			else {
				if (i != path->m) {
					pnl_mat_set(path, i, j, pnl_mat_get(path, i - 1, j) * exp((pnl_mat_get(pathInterest, i, j) - pnl_vect_get(dividend_, j) - (sigmaJ*sigmaJ) / 2) * timestep + sigmaJ * sqrtTimestep * pnl_vect_scalar_prod(V, G)));
				}
			}
		}
	}

	/** Lib�ration de la m�moire */
	pnl_vect_free(&W);
	pnl_vect_free(&G);
	pnl_vect_free(&V);

}





/**
* Calcule une trajectoire du sous-jacent connaissant le
* pass� jusqu' � la date t
*
* @param[out] path  contient une trajectoire du sous-jacent
* donn�e jusqu'� l'instant t par la matrice past
* @param[in] t date jusqu'� laquelle on connait la trajectoire.
* t n'est pas forc�ment une date de discr�tisation
* @param[in] nbTimeSteps nombre de pas de constatation
* @param[in] T date jusqu'� laquelle on simule la trajectoire
* @param[in] past trajectoire r�alis�e jusqu'a la date t
*/
void BlackScholesMertonModel::asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past, const PnlMat *pastInterest) {
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
	//interest_->interest(pathInterest, t, T, nbTimeSteps, rng, pastInterest);
	//updateTrend(pathInterest);
	/** Initialisation des param�tres */
	double timeSpend = 0;
	int counter = 0;

	/** Remplissage de la matrice path par la matrice past jusqu'� t*/
	while ((t >= timeSpend)/*&&( timeSpend < T)*/) {
		pnl_mat_get_row(V, past, counter);
		pnl_mat_set_row(path, V, counter);
		timeSpend += timestep;
		counter += 1;
	}

	// On r�cup�re St
	pnl_mat_get_row(W, past, past->m - 1);

	/** Calcul de la matrice des trajectoires des actifs */
	for (int i = counter; i < path->m; i++) {
		pnl_vect_rng_normal(G, size_, rng);
		for (int j = 0; j < path->n; j++) {
			pnl_mat_get_row(V, chol_, j);
			sigmaJ = pnl_vect_get(sigma_, j);
			if (i == counter) {
				// on place St en premi�re ligne
				pnl_mat_set(path, i, j, pnl_vect_get(W, j) * exp((pnl_mat_get(pathInterest, i, j) - pnl_vect_get(dividend_, j) - (sigmaJ*sigmaJ) / 2) * (timeSpend - t) + sigmaJ * sqrt(timeSpend - t) * pnl_vect_scalar_prod(V, G)));
			}
			else {
				if (i != path->m) {
					pnl_mat_set(path, i, j, pnl_mat_get(path, i - 1, j) * exp((pnl_mat_get(pathInterest, i, j) - pnl_vect_get(dividend_, j) - (sigmaJ*sigmaJ) / 2) * timestep + sigmaJ * sqrtTimestep * pnl_vect_scalar_prod(V, G)));
				}
			}
		}
	}

	/** Lib�ration de la m�moire */
	pnl_vect_free(&W);
	pnl_vect_free(&G);
	pnl_vect_free(&V);

}


