#pragma once
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "InterestRateModel.hpp"
#define DLLEXP   __declspec( dllexport )

namespace Computations {
	class VasicekModel : public InterestRateModel
	{
	public:
		int size_; /// nombre de taux d'intérêt différent à prendre en compte
		PnlVect *speedReversion_;
		PnlVect *longTermMean_;
		PnlVect *volatilities_;
		PnlMat *corr_;
		VasicekModel(); /// Constructeur par défaut
		VasicekModel(int size, double r, PnlVect *speedReversion, PnlVect *longTermMean, PnlVect *volatilities, PnlMat *corr); /// Constructeur taux identiques
		VasicekModel(int size, PnlVect *rSpot, PnlVect *speedReversion, PnlVect *longTermMean, PnlVect *volatilities, PnlMat *corr); /// Constructeur complet
		VasicekModel(const VasicekModel &VCM); /// Constructeur par recopie
		~VasicekModel(); /// Destructeur
		void initalizeChol(); /// Initialisateur de la matrice de cholesky
		VasicekModel& operator = (const VasicekModel &VCM); /// Opérateur d'affectation =

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
	private : 
		PnlMat *chol_;
	};
}

/**
 * Calcule le premier drift de la trajectoire à entre t et ti+1
 *
 * @param[out] le drift de l'élément du vecteur
 * @param[in] le vecteur à drift
 */
double VCMfirstdrift(double vectorElement);

/**
 * Calcule le drift de la trajectoire à entre t et ti+1
 *
 * @param[out] le drift de l'élément du vecteur
 * @param[in] le vecteur à drift
 */
double VCMdrift(double vectorElement);