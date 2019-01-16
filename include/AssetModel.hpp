#pragma once
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "InterestRateModel.hpp"
#define DLLEXP   __declspec( dllexport )

namespace Computations {
	class AssetModel
	{
	// public:
	// 	int size_; /// nombre d'actifs du mod�le
	// 	InterestRateModel *interest_;
	// 	PnlMat *corr_;
	// 	PnlVect *sigma_; /// vecteur de volatilit�s
	// 	PnlVect *spot_; /*! valeurs initiales des sous-jacents		 */
	// 	PnlVect *trend_; /* trajectoire compl�te des taux europ�ens mise � jour par l'interestRateModel*/
	// 	AssetModel(); /// Constructeur par d�faut
	// 	AssetModel(int size, InterestRateModel *interest, PnlMat *corr, PnlVect *sigma, PnlVect *spot); /// Constructeur sans prise en compte du pass� des taux d'int�r�t
	// 	AssetModel(const AssetModel &ASM); /// Constructeur par recopie
	// 	AssetModel& operator = (const AssetModel &ASM); /// op�rateur d'affectation
	// 	~AssetModel(); /// Destructeur

	// 	/**
	// 	 * G�n�re une trajectoire du mod�le et la stocke dans path
	// 	 *
	// 	 * @param[out] path contient une trajectoire du mod�le.
	// 	 * C'est une matrice de taille (nbTimeSteps+1) x d
	// 	 * @param[in] T  maturit�
	// 	 * @param[in] nbTimeSteps nombre de dates de constatation
	// 	 */
	// 	virtual void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng) = 0;

	// 	/**
	// 	 * Calcule une trajectoire du sous-jacent connaissant le
	// 	 * pass� jusqu' � la date t
	// 	 *
	// 	 * @param[out] path  contient une trajectoire du sous-jacent
	// 	 * donn�e jusqu'� l'instant t par la matrice past
	// 	 * @param[in] t date jusqu'� laquelle on connait la trajectoire.
	// 	 * t n'est pas forc�ment une date de discr�tisation
	// 	 * @param[in] nbTimeSteps nombre de pas de constatation
	// 	 * @param[in] T date jusqu'� laquelle on simule la trajectoire
	// 	 * @param[in] past trajectoire r�alis�e jusqu'a la date t
	// 	 */
	// 	virtual void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past) = 0;


	// 	/**
	// 	 * Calcule une trajectoire du sous-jacent connaissant le
	// 	 * pass� jusqu' � la date t
	// 	 *
	// 	 * @param[out] path  contient une trajectoire du sous-jacent
	// 	 * donn�e jusqu'� l'instant t par la matrice past
	// 	 * @param[in] t date jusqu'� laquelle on connait la trajectoire.
	// 	 * t n'est pas forc�ment une date de discr�tisation
	// 	 * @param[in] nbTimeSteps nombre de pas de constatation
	// 	 * @param[in] T date jusqu'� laquelle on simule la trajectoire
	// 	 * @param[in] past trajectoire r�alis�e jusqu'a la date t
	// 	 */
	// 	virtual void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past, const PnlMat *pastInterest) = 0;

	// 	/**
	// 	* Shift d'une trajectoire du sous-jacent
	// 	*
	// 	* @param[in]  path contient en input la trajectoire
	// 	* du sous-jacent
	// 	* @param[out] shift_path contient la trajectoire path
	// 	* dont la composante d a �t� shift�e par (1+h)
	// 	* � partir de la date t.
	// 	* @param[in] t date � partir de laquelle on shift
	// 	* @param[in] h pas de diff�rences finies
	// 	* @param[in] d indice du sous-jacent � shifter
	// 	* @param[in] timestep pas de constatation du sous-jacent
	// 	*/
	// 	void shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep);

	// 	/* Met � jour le trend � chaque simulation, ce dernier est utile dans plusieurs classe c'est pour cela qu'on le stocke
	// 	* @param[in] path contenant tous les taux d'int�r�ts de fa�on unique, la premi�re colonne doit �tre la colonne du taux d'int�r�t europ�en
	// 	*/
	// 	void updateTrend(PnlMat *pathInterest);

	// private : 
	// 	PnlMat *chol_;
	// 	/**
	// 	* permet d'inialiser la matrice de cholesky
	// 	*/
	// 	void initalizeChol();
	};
}