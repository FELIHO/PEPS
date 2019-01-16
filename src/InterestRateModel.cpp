#include "InterestRateModel.hpp"
#include "pch.h"
#include <math.h>
#include <iostream>
#include <stdexcept>
using namespace std;
using namespace Computations;

/**
Constructeur par défaut
*/
InterestRateModel::InterestRateModel()
{
	size_ = pnl_vect_create_from_zero(0); /// nombre d'actif du modèle
	rSpot_ = pnl_vect_create_from_zero(0); /// taux d'intérêt
}


/**
Constructeur avec taux d'intérêt identique
*/
InterestRateModel::InterestRateModel(int size, double r)
{
	size_ = pnl_vect_create_from_scalar(1, size);
	rSpot_ = pnl_vect_create_from_scalar(1, r); /// vecteur de taux d'intérêt pour les actions internationnales
}

/**
Constructeur complet
*/
InterestRateModel::InterestRateModel(PnlVect *size, PnlVect *r)
{	
	if (size_->size != rSpot_->size) {
		throw length_error("size_ attribute and rSpot_ attribute must be the same");
	}
	size_ = pnl_vect_copy(size); /// nombre d'actifs du modèle
	rSpot_ = pnl_vect_copy(r); /// vecteur de taux d'intérêt pour les actions internationnales
}

/**
Constructeur par recopie.
*/
InterestRateModel::InterestRateModel(const InterestRateModel &IRM) {
	size_ = pnl_vect_copy(IRM.size_);
	rSpot_ = pnl_vect_copy(IRM.rSpot_);
}

/**
Destructeur
*/

InterestRateModel::~InterestRateModel()
{
	pnl_vect_free(&size_);
	pnl_vect_free(&rSpot_);
}


/** Methode d'affectation d'un InterestRateModel
* @param[in] une image de la classe InterestRateModel à affecter.
* @param[out] la même référence InterestRateModel avec les mêmes paramètres que l'entrée
*/
InterestRateModel& InterestRateModel::operator = (const InterestRateModel &IRM) //le const c'est pour traduire le fait que cet opérateur ne modifie pas le Dvector
{
	size_ = IRM.size_;
	rSpot_ = IRM.rSpot_;
	return *this;
}


void InterestRateModel::makeCompletePathInterest(PnlMat *pathInterest) {

	PnlMat* pathInterestComplete = pnl_mat_create(pathInterest->m, pnl_vect_sum(size_));
	PnlVect* oneInterestPath = pnl_vect_new();
	int counter = 0;
	for (int i = 0; i < pathInterest->n; i++) {
		pnl_mat_get_col(oneInterestPath, pathInterest, i);
		for (int j = 0; j < pnl_vect_get(size_, i); j++) {
			pnl_mat_set_col(pathInterestComplete, oneInterestPath, counter);
			counter++;
		}
	}
	pathInterest = pnl_mat_copy(pathInterestComplete);
	pnl_vect_free(&oneInterestPath);
	pnl_mat_free(&pathInterestComplete);
}



double squared(double vectorElement) {
	return vectorElement * vectorElement;
}

double unsquared(double vectorElement) {
	return sqrt(vectorElement);
}

double oneUnaryMinus(double vectorElement) {
	return 1 - vectorElement;
}

double computeFirstTimeSteps(double timeSteps, double t) {
	int k = 0;
	while (t > k * timeSteps) {
		k = k + 1;
	}
	return (k * timeSteps) - t;
}
