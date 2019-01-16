#include "BlackScholesMertonModel.hpp"
#include "pch.h"
#include <math.h>
#include <iostream>
#include <stdexcept>
using namespace std;
using namespace Computations;

/**
Constructeur par défaut
*/
BlackScholesMertonModel::BlackScholesMertonModel() : AssetModel()
{	
	dividend_ = pnl_vect_new();
}



/**
Constructeur complet
*/
BlackScholesMertonModel::BlackScholesMertonModel(int size, InterestRateModel *interest, PnlMat *corr, PnlVect *sigma, PnlVect *spot, PnlVect *dividend) : AssetModel(size, interest, corr, sigma, spot)
{
	dividend_ = pnl_vect_copy(dividend);
}



/**
Destructeur
*/

BlackScholesMertonModel::~BlackScholesMertonModel()
{	
	interest_->~InterestRateModel();
	pnl_vect_free(&sigma_);
	pnl_vect_free(&spot_);
	pnl_vect_free(&trend_);
	pnl_vect_free(&dividend_);
	pnl_mat_free(&chol_);
}


/** Methode d'affectation d'un BlackScholesMertonModel
* @param[in] une image de la classe BlackScholesMertonModel à affecter.
* @param[out] la même référence BlackScholesMertonModel avec les mêmes paramètres que l'entrée
*/
BlackScholesMertonModel& BlackScholesMertonModel::operator = (const BlackScholesMertonModel &BSMM) //le const c'est pour traduire le fait que cet opérateur ne modifie pas le Dvector
{
	size_ = BSMM.size_;
	interest_ = BSMM.interest_;
	corr_ = BSMM.corr_;
	sigma_ = BSMM.sigma_;
	spot_ = BSMM.spot_;
	trend_ = BSMM.trend_;
	dividend_ = BSMM.dividend_;
	chol_ = BSMM.chol_;
	return *this;
}

void BlackScholesMertonModel::initalizeChol() {
	/** Initialisation de la matrice de corrélation et de la matrice de cholesky*/

	/** Validation de la matrice de corrélation */
	PnlVect *eigenValues = pnl_vect_create(size_);
	PnlMat *eigenVectors = pnl_mat_create(size_, size_);
	bool validatedRho = false;

	pnl_mat_eigen(eigenValues, eigenVectors, corr_, 1);
	for (int i = 0; i < size_; i++) {
		if (GET(eigenValues, i) <= 0) {
			validatedRho = true;
			break;
		}
	}

	pnl_vect_free(&eigenValues);
	pnl_mat_free(&eigenVectors);
	//lever une exeption si  la matrice de corrélation n'est pas diagonalisable
	if (validatedRho) {
		throw length_error("Invalid matrix");
	}

	chol_ = pnl_mat_copy(corr_);
	pnl_mat_chol(chol_);

}

/**
* Génère une trajectoire du modèle et la stocke dans path
*
* @param[out] path contient une trajectoire du modèle.
* C'est une matrice de taille (nbTimeSteps+1) x d
* @param[in] T  maturité
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
	interest_->interest(pathInterest, T, nbTimeSteps, rng);
	updateTrend(pathInterest);

	// Première ligne
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

	/** Libération de la mémoire */
	pnl_vect_free(&G);
	pnl_vect_free(&V);

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
	interest_->interest(pathInterest, T, nbTimeSteps, rng);
	updateTrend(pathInterest);

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
				pnl_mat_set(path, i, j, pnl_vect_get(W, j) * exp((pnl_mat_get(pathInterest, i, j) - pnl_vect_get(dividend_, j) - (sigmaJ*sigmaJ) / 2) * (timeSpend - t) + sigmaJ * sqrt(timeSpend - t) * pnl_vect_scalar_prod(V, G)));
			}
			else {
				if (i != path->m) {
					pnl_mat_set(path, i, j, pnl_mat_get(path, i - 1, j) * exp((pnl_mat_get(pathInterest, i, j) - pnl_vect_get(dividend_, j) - (sigmaJ*sigmaJ) / 2) * timestep + sigmaJ * sqrtTimestep * pnl_vect_scalar_prod(V, G)));
				}
			}
		}
	}

	/** Libération de la mémoire */
	pnl_vect_free(&W);
	pnl_vect_free(&G);
	pnl_vect_free(&V);

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
	interest_->interest(pathInterest, t, T, nbTimeSteps, rng, pastInterest);
	updateTrend(pathInterest);
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
				pnl_mat_set(path, i, j, pnl_vect_get(W, j) * exp((pnl_mat_get(pathInterest, i, j) - pnl_vect_get(dividend_, j) - (sigmaJ*sigmaJ) / 2) * (timeSpend - t) + sigmaJ * sqrt(timeSpend - t) * pnl_vect_scalar_prod(V, G)));
			}
			else {
				if (i != path->m) {
					pnl_mat_set(path, i, j, pnl_mat_get(path, i - 1, j) * exp((pnl_mat_get(pathInterest, i, j) - pnl_vect_get(dividend_, j) - (sigmaJ*sigmaJ) / 2) * timestep + sigmaJ * sqrtTimestep * pnl_vect_scalar_prod(V, G)));
				}
			}
		}
	}

	/** Libération de la mémoire */
	pnl_vect_free(&W);
	pnl_vect_free(&G);
	pnl_vect_free(&V);

}


