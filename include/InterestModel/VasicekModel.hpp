#pragma once
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "InterestRateModel.hpp"


	class VasicekModel : public InterestRateModel
	{
	public:
		PnlVect *size_; /// nombre de taux d'int�r�t diff�rent � prendre en compte
		PnlVect *speedReversion_;
		PnlVect *longTermMean_;
		PnlVect *volatilities_;
		PnlMat *corr_;
		VasicekModel(); /// Constructeur par d�faut
		VasicekModel(int size, double r, PnlVect *speedReversion, PnlVect *longTermMean, PnlVect *volatilities, PnlMat *corr); /// Constructeur taux identiques
		VasicekModel(PnlVect *size, PnlVect *rSpot, PnlVect *speedReversion, PnlVect *longTermMean, PnlVect *volatilities, PnlMat *corr); /// Constructeur complet
		VasicekModel(const VasicekModel &VCM); /// Constructeur par recopie
		~VasicekModel(); /// Destructeur
		void initalizeChol(); /// Initialisateur de la matrice de cholesky
		VasicekModel& operator = (const VasicekModel &VCM); /// Op�rateur d'affectation =

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
	private :
		PnlMat *chol_;
	};

/**
 * Calcule le premier drift de la trajectoire � entre t et ti+1
 *
 * @param[out] le drift de l'�l�ment du vecteur
 * @param[in] le vecteur � drift
 */
double VCMfirstdrift(double vectorElement);

/**
 * Calcule le drift de la trajectoire � entre t et ti+1
 *
 * @param[out] le drift de l'�l�ment du vecteur
 * @param[in] le vecteur � drift
 */
double VCMdrift(double vectorElement);
