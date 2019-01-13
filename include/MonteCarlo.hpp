#pragma once
#include "Option.hpp"
#include "AssetModel.hpp"
#include "DeltaCompute.hpp"
#include "pnl/pnl_random.h"
#include <iostream>
#include <string>
#define DLLEXP   __declspec( dllexport )

namespace Computations {
	class MonteCarlo
	{
	public:
		DeltaCompute *delta_; /*! pointeur vers la technique du calcul du delta */
		AssetModel *mod_; /*! pointeur vers le modèle */
		Option *opt_; /*! pointeur sur l'option */
		PnlRng *rng_; /*! pointeur sur le générateur */
		double fdStep_; /*! pas de différence finie */
		size_t nbSamples_; /*! nombre de tirages Monte Carlo */

		MonteCarlo();
		/*MonteCarlo& operator = (const MonteCarlo &BSM);
		MonteCarlo(const MonteCarlo &MC);*/
		~MonteCarlo();
		MonteCarlo(DeltaCompute *delta, AssetModel *mod, Option *opt, int nbSamples, PnlRng *rng, double fdStep);

		/**
		 * Calcule le prix de l'option à la date 0
		 *
		 * @param[out] prix valeur de l'estimateur Monte Carlo
		 * @param[out] ic largeur de l'intervalle de confiance
		 */
		DLLEXP void price(double &prix, double &ic);

		/**
		 * Calcule le prix de l'option à la date t
		 *
		 * @param[in]  past contient la trajectoire du sous-jacent
		 * jusqu'à l'instant t
		 * @param[in] t date à laquelle le calcul est fait
		 * @param[out] prix contient le prix
		 * @param[out] ic contient la largeur de l'intervalle
		 * de confiance sur le calcul du prix
		 */
		DLLEXP void price(const PnlMat *past, double t, double &prix, double &ic);

		/**
		 * Calcule le delta de l'option à la date t
		 *
		 * @param[in]  past contient la trajectoire du sous-jacent
		 * jusqu'à l'instant t
		 * @param[in] t date à laquelle le calcul est fait
		 * @param[out]  delta contient le delta
		 * @param[out] ic contient la largeur de l'intervalle
		 * de confiance sur le calcul du delta
		 */
		DLLEXP void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic);


		DLLEXP void Profit_and_loss(const PnlMat*past, double &PL, const int H);
	};
}