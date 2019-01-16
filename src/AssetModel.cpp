#include "AssetModel.hpp"
#include "pch.h"
#include <math.h>
#include <iostream>
#include <stdexcept>
#include "SimpleRateModel.hpp"
using namespace std;
using namespace Computations;

/**
Constructeur par d�faut
*/
AssetModel::AssetModel()
{
	size_ = 0;
    sigma_ = pnl_vect_new (); /// vecteur de volatilit�s
    spot_ = pnl_vect_new (); /// valeurs initiales des sous-jacents
	corr_ = pnl_mat_new();
	trend_ = pnl_vect_new();
	interest_ = new SimpleRateModel(size_, 0.0);
}



/**
Constructeur complet
*/
AssetModel::AssetModel(int size, InterestRateModel *interest, PnlMat *corr, PnlVect *sigma, PnlVect *spot)
{ 
  size_ = size; /// nombre d'actifs du mod�le
  interest_ = interest; /// taux d'int�r�t
  corr_ = pnl_mat_copy(corr); /// param�tre de corr�lation
  sigma_ = pnl_vect_copy(sigma); /// vecteur de volatilit�s
  spot_ = pnl_vect_copy(spot);
  trend_ = pnl_vect_new();
  initalizeChol();
}


/**
Constructeur par recopie.
*/
AssetModel::AssetModel(const AssetModel &ASM)
{
  size_ = ASM.size_;
  interest_ = ASM.interest_;
  spot_ = pnl_vect_copy(ASM.spot_);
  corr_ = pnl_mat_copy(ASM.corr_); /// param�tre de corr�lation
  chol_ = pnl_mat_copy(ASM.chol_);
  trend_ = pnl_vect_copy(ASM.trend_);
}

/**
Destructeur
*/

AssetModel::~AssetModel()
{
  interest_->~InterestRateModel();
  pnl_vect_free(&sigma_);
  pnl_vect_free(&trend_);
  pnl_vect_free(&spot_);
  pnl_mat_free(&corr_);
  pnl_mat_free(&chol_);
}


/** Methode d'affectation d'un AssetModel
* @param[in] une image de la classe AssetModel � affecter.
* @param[out] la m�me r�f�rence AssetModel avec les m�mes param�tres que l'entr�e
*/
AssetModel& AssetModel::operator = (const AssetModel &ASM) //le const c'est pour traduire le fait que cet op�rateur ne modifie pas le Dvector
{
	size_ = ASM.size_;
	interest_ = ASM.interest_;
	spot_ = ASM.spot_;
	corr_ = ASM.corr_; /// param�tre de corr�lation
	chol_ = ASM.chol_;
	trend_ = ASM.trend_;
	return *this;
}

void AssetModel::initalizeChol(){

	/** Validation de la matrice de corr�lation */
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
	//lever une exeption si rho n'est pas dans l'intervalle [(1/(1-D)) , 1] ou la matrice de corrlation n'est pas diagonalisable
	if (validatedRho) {
		throw length_error("Invalid matrix");
	}

	chol_ = pnl_mat_copy(corr_);
	pnl_mat_chol(chol_);

}



void AssetModel::shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep) {
	if ((shift_path->m != path->m) || (path->n != shift_path->n)) {
		throw length_error("Invalide taille");
	}
	/** Initialisation des param�tres */
	int indiceRow = 0;
	double timeSpend = 0.0;
	while (timeSpend <= t) {
		timeSpend += timestep;
		indiceRow += 1;
	}

	/** Pour r�cup�rer directement les colonnes de la matrice avec un indice diff�rent de d */
	PnlVect *U = pnl_vect_create(path->m);

	for (int j = 0; j < path->n; j++) {
		if ((j + 1) != d) {
			/** On r�cup�re les colonnes diff�rentes de d */
			pnl_mat_get_col(U, path, j);
			pnl_mat_set_col(shift_path, U, j);
		}
		else {
			/* On remplit la derni�re colonne d */
			for (int i = 0; i < path->m; i++) {
				double shift = 1.0;
				/** Si l'indice est apr�s t on fait un d�cale de 1+h*/
				if (i >= indiceRow) {
					shift += h;
				}
				pnl_mat_set(shift_path, i, j, pnl_mat_get(path, i, j)*shift);
			}
		}
	}
	pnl_vect_free(&U);
}


void AssetModel::updateTrend(PnlMat *pathInterest) {
	// PAR CONVENTION LE TAUX D'INTERET EUROPEEN EST LE PREMIER DANS LA MATRICE PATHINTEREST
	 pnl_mat_get_col(trend_, pathInterest, 0);
}