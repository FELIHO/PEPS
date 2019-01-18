#pragma once
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "InterestRateModel.hpp"


namespace Computations {
	class SimpleRateModel : public InterestRateModel
	{
	public:
		PnlVect *size_; /// nombre de taux d'int�r�t diff�rent � prendre en compte
		SimpleRateModel(); /// Constructeur par d�faut
		SimpleRateModel(int size, double r); /// Constructeur taux identiques
		SimpleRateModel(PnlVect *size, PnlVect *r); /// Constructeur complet
		SimpleRateModel(const SimpleRateModel &SRM); /// Constructeur par recopie
		~SimpleRateModel(); /// Destructeur
		SimpleRateModel& operator = (const SimpleRateModel &SRM); /// Op�rateur d'affectation =

		/**
		 * G�n�re une trajectoire du mod�le de taux et la stocke dans path
		 *
		 * @param[out] path contient une trajectoire du mod�le.
		 * C'est une matrice de taille (nbTimeSteps+1) x d
		 * @param[in] T  maturit�
		 * @param[in] nbTimeSteps nombre de dates de constatation
		 */
		void interest(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);

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
		void interest(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past);

	};
}
