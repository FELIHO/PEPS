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
		 * double r_ : taux d'intérêt pour chaque actif
		 */
		PnlVect *r_;

		/**
		 *  double rho_ : matrice de corrélation
		 */
		PnlMat *rho_;

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
		PnlMat *trend_;

		/**
		 *  PnlMat* choleskyFactor : récipient de la factorisation de Cholesky
		 */
		PnlMat* chol_;

		/**
		Constructeur par défaut
		*/
		BlackScholesModel();

		/**
		Constructeur par copie
		*/
		BlackScholesModel(const BlackScholesModel &BSM);

		/** Methode d'affectation d'un BlackScholesModel
		* @param[in] une image de la classe BlackScholesModel à affecter.
		* @param[out] la même référence BlackScholesModel avec les mêmes paramètres que l'entrée
		*/
		BlackScholesModel& operator = (const BlackScholesModel &BSM); /// Opérateur d'affectation =

		/**
		Destructeur
		*/
		~BlackScholesModel();

		/**
		 * \brief Constructeur de la classe BlackScholesModel
		 *
		 * @param[in] size_ nombre d'actifs du modèle
		 * @param[in] r_ taux d'intérêt
		 * @param[in] rho_ paramètre de corrélation
		 * @param[in] sigma_ vecteur de volatilités
		 * @param[in] spot_ valeurs initiales des sous-jacents
		 */
		BlackScholesModel(int size, PnlVect *r , PnlMat *rho, PnlVect *sigma, PnlVect *spot);

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
		BlackScholesModel(int size, PnlVect *r , PnlMat *rho, PnlVect *sigma, PnlVect *spot, PnlMat *trend);

		/**
		 * Génère une trajectoire du modèle et la stocke dans path
		 *
		 * @param[out] path contient une trajectoire du modèle.
		 * C'est une matrice de taille (nbTimeSteps+1) x d
		 * @param[in] T  maturité
		 * @param[in] nbTimeSteps nombre de dates de constatation
		 */
		void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);

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
		void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past);

		/**
		 * Shift d'une trajectoire du sous-jacent
		 *
		 * @param[in]  path contient en input la trajectoire
		 * du sous-jacent
		 * @param[out] shift_path contient la trajectoire path
		 * dont la composante d a été shiftée par (1+h)
		 * à partir de la date t.
		 * @param[in] t date à partir de laquelle on shift
		 * @param[in] h pas de différences finies
		 * @param[in] d indice du sous-jacent à shifter
		 * @param[in] timestep pas de constatation du sous-jacent
		 */
		void shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep);

		// /**
		//  * Calcule une trajectoire du sous-jacent connaissant le
		//  * passé jusqu' à la date t
		//  *
		//  * @param[out] path  contient une trajectoire du sous-jacent
		//  * donnée jusqu'à l'instant t par la matrice past
		//  * @param[in] t date jusqu'à laquelle on connait la trajectoire.
		//  * t n'est pas forcément une date de discrétisation
		//  * @param[in] nbTimeSteps nombre de pas de constatation
		//  * @param[in] T date jusqu'à laquelle on simule la trajectoire
		//  * @param[in] past trajectoire réalisée jusqu'a la date t
		//  * @param[in] pastInterest trajectoire réalisée par les taux d'intéret jusqu'a la date t
		//  */
		// void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past, const PnlMat *pastInterest);
    
		/**
		 * Génère une trajectoire du modèle selon la probabilité risque neutre et la stocke dans market
		 *
		 * @param[out] market contient une trajectoire du modèle.
		 * C'est une matrice de taille (nbTimeSteps+1) x d
		 * @param[in] trend tendance du marché
		 * C'est un vecteur de taille nombre d'actifs
		 * @param[in] T  maturité
		 * @param[in] H nombre de dates d'observation
		 */
		void simul_market(PnlMat* market, double T, int H, PnlRng *rng);


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
		void simulateAsset(PnlMat *path, double timestep, int nbTimeSteps, PnlRng *rng, PnlVect* r);
		};
}