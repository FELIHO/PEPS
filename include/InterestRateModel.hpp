#pragma once
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#define DLLEXP   __declspec( dllexport )

namespace Computations {
	class InterestRateModel
	{
	public:
		int size_; /// nombre de taux d'int�r�t diff�rent � prendre en compte
		PnlVect *rSpot_; /// taux d'int�r�t
		InterestRateModel(); /// Constructeur par d�faut
		InterestRateModel(int size, double r); /// Constructeur taux identiques
		InterestRateModel(int size, PnlVect *r_); /// Constructeur complet
		InterestRateModel(const InterestRateModel &IRM); /// Constructeur par recopie
		~InterestRateModel(); /// Destructeur
		InterestRateModel& operator = (const InterestRateModel &IRM); /// Op�rateur d'affectation =

		/**
		 * G�n�re une trajectoire du mod�le de taux et la stocke dans path
		 *
		 * @param[out] path contient une trajectoire du mod�le.
		 * C'est une matrice de taille (nbTimeSteps+1) x d
		 * @param[in] T  maturit�
		 * @param[in] nbTimeSteps nombre de dates de constatation
		 */
		virtual void interest(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng) = 0;

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
		virtual void interest(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past) = 0;


	};
}

/**
 * Calcule fonction utile pour retourner le carr� d'un vecteur
 *
 * @param[out] le carr� de l'�l�ment du vecteur
 * @param[in] l'�l�ment � mettre au carr�
 */
double squared(double vectorElement);

/**
 * Calcule fonction utile pour retourner la racine d'un vecteur
 *
 * @param[out] la racine de l'�l�ment du vecteur
 * @param[in] l'�l�ment � mettre au carr�
 */
double unsquared(double vectorElement);

/**
 * Calcule fonction utile pour retourner 1 - x le vecteur
 *
 * @param[out] l'�l�ment y = f(x) = 1 -x
 * @param[in] l'�l�ment x
 */
double oneUnaryMinus(double vectorElement);

/**
 * Cette fonction permet de retrouver � quel instant t
 *
 * @param[out] l'�l�ment y = f(x) = 1 -x
 * @param[in] l'�l�ment x
 */
double computeFirstTimeSteps(double timeSteps, double t);

