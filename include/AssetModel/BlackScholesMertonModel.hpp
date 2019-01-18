#pragma once
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "BlackScholesModel.hpp"
#include "InterestRateModel.hpp"


namespace Computations {
	class BlackScholesMertonModel : public BlackScholesModel
	{
	public:
		PnlVect *dividend_;
		BlackScholesMertonModel(); /// Constructeur par d�faut
		BlackScholesMertonModel(int size, PnlVect *r, PnlMat *rho, PnlVect *sigma, PnlVect *spot, PnlVect *dividend);
		BlackScholesMertonModel(int size, PnlVect *r, PnlMat *rho, PnlVect *sigma, PnlVect *spot, PnlMat *trend, PnlVect *dividend);
		//BlackScholesMertonModel(int size, InterestRateModel *interest, PnlMat *corr, PnlVect *sigma, PnlVect *spot, PnlVect *dividend_); /// Constructeur complet
		~BlackScholesMertonModel(); /// Destructeur
		BlackScholesMertonModel& operator = (const BlackScholesMertonModel &BSM); /// Op�rateur d'affectation =


		/**
		 * G�n�re une trajectoire du mod�le et la stocke dans path
		 *
		 * @param[out] path contient une trajectoire du mod�le.
		 * C'est une matrice de taille (nbTimeSteps+1) x d
		 * @param[in] T  maturit�
		 * @param[in] nbTimeSteps nombre de dates de constatation
		 */
		 void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);

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
		 void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past);

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
		 void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past, const PnlMat *pastInterest);

	private:
	};
}
