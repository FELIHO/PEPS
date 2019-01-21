#pragma once
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_cdf.h"
#include "Option/Basket.hpp"
#include <cmath>
#include <assert.h>

namespace Computations {
	class QuantoBasket : public Basket
	{

	public:
		/**
		* Default Constructor
		* Initialise everything to zero. Shouldn't be used.
		*/
		 QuantoBasket();

		/*
		* Copy Constructor
		*/
		 QuantoBasket(const QuantoBasket &C);

		/*
		* Affectation
		*/
		 QuantoBasket& operator=(const QuantoBasket &C);

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

}