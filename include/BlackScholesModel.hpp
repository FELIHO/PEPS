#pragma once
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

#include <math.h>
#include <assert.h>
#include <sstream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <stdexcept>

#include "RandomGen.hpp"
#include "FakeRnd.hpp"
#include "PnlRand.hpp"
#define DLLEXP   __declspec( dllexport )

namespace Computations {
	class BlackScholesModel : public AssetModel
	{
	public:
		/**
		 * int size_ : nombre d'actifs du modèle
		 */
		int size_;

		/**
		 * double r_ : taux d'intérêt
		 */
		double r_;

		/**
		 *  double rho_ : paramètre de corrélation
		 */
		double rho_;

		/**
		 *  PnlVect *sigma_ : vecteur de volatilités
		 */
		PnlVect *sigma_;

		/**
		 *  PnlVect *spot_ : valeurs initiales des sous-jacents
		 */
		PnlVect *spot_;

		/**
		 *  PnlVect *trend_ : tendance du marche
		 */
		PnlVect *trend_;

		/**
		 *  PnlMat* choleskyFactor : récipient de la factorisation de Cholesky
		 */
		PnlMat* choleskyFactor;

		/**
		 * \brief Constructeur de la classe BlackScholesModel
		 *
		 * @param[in] size_ nombre d'actifs du modèle
		 * @param[in] r_ taux d'intérêt
		 * @param[in] rho_ paramètre de corrélation
		 * @param[in] sigma_ vecteur de volatilités
		 * @param[in] spot_ valeurs initiales des sous-jacents
		 */
		BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot);

		/**
		 * \brief Constructeur de la classe BlackScholesModel
		 *
		 * @param[in] size_ nombre d'actifs du modèle
		 * @param[in] r_ taux d'intérêt
		 * @param[in] rho_ paramètre de corrélation
		 * @param[in] sigma_ vecteur de volatilités
		 * @param[in] spot_ valeurs initiales des sous-jacents
		 * @param[in] trend_ vecteur de v
		 */
		BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot, PnlVect *trend_);


		/**
		Constructeur par défaut
		*/
		BlackScholesModel();

		/**
		Constructeur par copie
		*/
		BlackScholesModel(const BlackScholesModel &BlackScholesModelACopier);


		/**
		Destructeur
		*/
		~BlackScholesModel();

		/** Methode d'affectation d'un BlackScholesModel
		* @param[in] une image de la classe BlackScholesModel à affecter.
		* @param[out] la même référence BlackScholesModel avec les mêmes paramètres que l'entrée
		*/
		BlackScholesModel& operator = (const BlackScholesModel &BSM); /// Opérateur d'affectation =

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
		 * @param[in] pastInterest trajectoire réalisée par les taux d'intéret jusqu'a la date t
		 */
		DLLEXP void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past, const PnlMat *pastInterest);


	private :
		/**
		* Concatène deux PnlMat 
		* @param[out] res matrice résultat de la concaténation
		* @param[in] mat1 1ere matrice à concaténer
		* @param[in] mat2 1ere matrice à concaténer
		*
		*/  
		void concatenationMatrice(PnlMat* res,const PnlMat *mat1, const PnlMat *mat2);


		/**
		* Simule les cours des sous-jacents selon le modèle de Black-Scholes
		*
		* @param[out] path matrice contenant les données des cours S_t
		* @param[in] timestep pas de constatation
		* @param[in] nbTimeSteps nombre de pas de constatation
		* @param[in] rng Générateur de nombre aléatoire
		* @param[in] r vecteur des taux d'intérêts des sous-jacents
		*/
		void simulateAsset(PnlMat *path, double timestep, int nbTimeSteps, RandomGen *rng, PnlVect* r);
		};
}