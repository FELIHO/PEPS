#include "InterestRateModel.hpp"
#include <math.h>
#include <iostream>
#include <stdexcept>
using namespace std;
using namespace Computations;

/**
Constructeur par d�faut
*/
InterestRateModel::InterestRateModel()
{
	size_ = 0; /// nombre d'actifs du mod�le
	rSpot_ = pnl_vect_create_from_zero(0); /// taux d'int�r�t
}


/**
Constructeur avec taux d'int�r�t identique
*/
InterestRateModel::InterestRateModel(int size, double r)
{
	size_ = size; /// nombre d'actifs du mod�le
	rSpot_ = pnl_vect_create_from_scalar(size_, r); /// vecteur de taux d'int�r�t pour les actions internationnales
}

/**
Constructeur complet
*/
InterestRateModel::InterestRateModel(int size, PnlVect *r)
{
	size_ = size; /// nombre d'actifs du mod�le
	rSpot_ = pnl_vect_copy(r); /// vecteur de taux d'int�r�t pour les actions internationnales
}

/**
Constructeur par recopie.
*/
InterestRateModel::InterestRateModel(const InterestRateModel &IRM) {
	size_ = IRM.size_;
	rSpot_ = pnl_vect_copy(IRM.rSpot_);
}

/**
Destructeur
*/

InterestRateModel::~InterestRateModel()
{
	pnl_vect_free(&rSpot_);
}


/** Methode d'affectation d'un InterestRateModel
* @param[in] une image de la classe InterestRateModel � affecter.
* @param[out] la m�me r�f�rence InterestRateModel avec les m�mes param�tres que l'entr�e
*/
InterestRateModel& InterestRateModel::operator = (const InterestRateModel &IRM) //le const c'est pour traduire le fait que cet op�rateur ne modifie pas le Dvector
{
	size_ = IRM.size_;
	rSpot_ = IRM.rSpot_;
	return *this;
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