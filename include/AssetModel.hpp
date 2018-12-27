#pragma once
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "InterestRateModel.hpp"
#define DLLEXP   __declspec( dllexport )

namespace Computations {
	class AssetModel
	{
	public:
		int size_; /// nombre d'actifs du modèle
		InterestRateModel *interest_;
		PnlMat *corr_;
		PnlVect *sigma_; /// vecteur de volatilités
		PnlVect *spot_; /// valeurs initiales des sous-jacents		

		AssetModel(); /// Constructeur par défaut
		AssetModel(int size, InterestRateModel *interest, PnlMat *corr, PnlVect *sigma, PnlVect *spot); /// Constructeur sans prise en compte du passé des taux d'intérêt
		AssetModel(const AssetModel &ASM); /// Constructeur par recopie
		
		~AssetModel(); /// Destructeur

		/**
		 * Génère une trajectoire du modèle et la stocke dans path
		 *
		 * @param[out] path contient une trajectoire du modèle.
		 * C'est une matrice de taille (nbTimeSteps+1) x d
		 * @param[in] T  maturité
		 * @param[in] nbTimeSteps nombre de dates de constatation
		 */
		virtual void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng) = 0;

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
		virtual void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past) = 0;


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
		virtual void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past, const PnlMat *pastInterest) = 0;

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

	private : 
		PnlMat *chol_;
		/**
		* permet d'inialiser la matrice de cholesky
		*/
		void initalizeChol();
	};
}