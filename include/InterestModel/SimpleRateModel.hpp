#pragma once
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "InterestRateModel.hpp"
#define DLLEXP   __declspec( dllexport )

namespace Computations {
	class SimpleRateModel : public InterestRateModel
	{
	public:
		PnlVect *size_; /// nombre de taux d'intérêt différent à prendre en compte
		SimpleRateModel(); /// Constructeur par défaut
		SimpleRateModel(int size, double r); /// Constructeur taux identiques
		SimpleRateModel(PnlVect *size, PnlVect *r); /// Constructeur complet
		SimpleRateModel(const SimpleRateModel &SRM); /// Constructeur par recopie
		~SimpleRateModel(); /// Destructeur
		SimpleRateModel& operator = (const SimpleRateModel &SRM); /// Opérateur d'affectation =

		/**
		 * Génère une trajectoire du modèle de taux et la stocke dans path
		 *
		 * @param[out] path contient une trajectoire du modèle.
		 * C'est une matrice de taille (nbTimeSteps+1) x d
		 * @param[in] T  maturité
		 * @param[in] nbTimeSteps nombre de dates de constatation
		 */
		void interest(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);

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
		void interest(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past);

	};
}
