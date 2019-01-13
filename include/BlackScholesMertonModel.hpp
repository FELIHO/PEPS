#pragma once
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "AssetModel.hpp"
#define DLLEXP   __declspec( dllexport )

namespace Computations {
	class BlackScholesMertonModel : public AssetModel
	{
	public:
		PnlVect *dividend_;
		BlackScholesMertonModel(); /// Constructeur par défaut
		BlackScholesMertonModel(int size, InterestRateModel *interest, PnlMat *corr, PnlVect *sigma, PnlVect *spot, PnlVect *dividend_); /// Constructeur complet
		~BlackScholesMertonModel(); /// Destructeur
		BlackScholesMertonModel& operator = (const BlackScholesMertonModel &BSM); /// Opérateur d'affectation =


		/**
		 * Génère une trajectoire du modèle et la stocke dans path
		 *
		 * @param[out] path contient une trajectoire du modèle.
		 * C'est une matrice de taille (nbTimeSteps+1) x d
		 * @param[in] T  maturité
		 * @param[in] nbTimeSteps nombre de dates de constatation
		 */
		DLLEXP void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);

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
		DLLEXP void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past);

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
		DLLEXP void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past, const PnlMat *pastInterest);

	private:
		PnlMat *chol_;
		/**
		* permet d'inialiser la matrice de cholesky
		*/
		void initalizeChol();
	};
}