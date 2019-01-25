#pragma once
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_cdf.h"
#include "Basket.hpp"
#include <cmath>
#include <assert.h>

	class QuantoBasket : public Option
	{

	public:

		// Attributes
		/**
		 * double strike_ : prix d'exercice de l'option
		 */
		double strike_; // strike K of the basket option
		/**
		 * PnlVect *weights_ : vecteur des coefficients de pondération des actifs sous-jacents
		 */
		PnlVect *weights_; // vector containing the proportions of each one of the D underlying asset

		/**
		Constructeur par défaut
		*/
		 QuantoBasket();

		/**
		*  \brief Constructeur par copie d'objet Basket
		*  @param[in] param source : l'objet Basket à copier
		*/
		 QuantoBasket(const QuantoBasket &QB);

		/** Methode d'affectation d'une Option Basket
		* @param[in] une image de l'Option Basket à affecter.
		* @param[out] la même référence Option Basket avec les mêmes paramètres que l'entrée
		*/
		 QuantoBasket& operator=(const QuantoBasket &QB);

		/**
		* Destructor
		*/
		 ~QuantoBasket();

	/**
	*  \brief Constructeur de la classe QuantoBasket permettant de préciser les caractéristiques du produits
	*  @param[in] T : Maturité de l'option
	*  @param[in] nbTimeSteps : nombre de dates de constatation des cours du sous-jacent
	*  @param[in] size : nombre d'actifs sous-jacents
	*  @param[in] strike : prix d'exercice de l'option
	*  @param[in] weights : vecteur des coefficients de pondération des actifs sous-jacents
	*/
	QuantoBasket(double T, int nbTimeSteps, int size, double strike, PnlVect *weights);

	virtual QuantoBasket* clone();

	virtual double payoff(const PnlMat *path, const PnlMat *pathChangeRate, const PnlVect * currency);

	};