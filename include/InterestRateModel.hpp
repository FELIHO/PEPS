#pragma once
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#define DLLEXP   __declspec( dllexport )

namespace Computations {
	class InterestRateModel
	{
	public:
		int size_; /// nombre de taux d'intérêt différent à prendre en compte
		PnlVect *rSpot_; /// taux d'intérêt
		InterestRateModel(); /// Constructeur par défaut
		InterestRateModel(int size, double r); /// Constructeur taux identiques
		InterestRateModel(int size, PnlVect *r_); /// Constructeur complet
		InterestRateModel(const InterestRateModel &IRM); /// Constructeur par recopie
		~InterestRateModel(); /// Destructeur
		InterestRateModel& operator = (const InterestRateModel &IRM); /// Opérateur d'affectation =

		/**
		 * Génère une trajectoire du modèle de taux et la stocke dans path
		 *
		 * @param[out] path contient une trajectoire du modèle.
		 * C'est une matrice de taille (nbTimeSteps+1) x d
		 * @param[in] T  maturité
		 * @param[in] nbTimeSteps nombre de dates de constatation
		 */
		virtual void interest(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng) = 0;

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
		virtual void interest(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past) = 0;


	};
}

/**
 * Calcule fonction utile pour retourner le carré d'un vecteur
 *
 * @param[out] le carré de l'élément du vecteur
 * @param[in] l'élément à mettre au carré
 */
double squared(double vectorElement);

/**
 * Calcule fonction utile pour retourner la racine d'un vecteur
 *
 * @param[out] la racine de l'élément du vecteur
 * @param[in] l'élément à mettre au carré
 */
double unsquared(double vectorElement);

/**
 * Calcule fonction utile pour retourner 1 - x le vecteur
 *
 * @param[out] l'élément y = f(x) = 1 -x
 * @param[in] l'élément x
 */
double oneUnaryMinus(double vectorElement);

/**
 * Cette fonction permet de retrouver à quel instant t
 *
 * @param[out] l'élément y = f(x) = 1 -x
 * @param[in] l'élément x
 */
double computeFirstTimeSteps(double timeSteps, double t);

