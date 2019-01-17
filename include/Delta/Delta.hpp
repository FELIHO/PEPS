#pragma once
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "Option/Option.hpp"
#include "AssetModel/BlackScholesModel.hpp"

namespace Computations {
	class Delta
	{
	public:
		/**
		Default Constructor
		*/
		Delta();
		/**
		Copy Constructor
		*/
		Delta(const Delta &D);
		/**
		Destructor
		*/
		~Delta();

		/**
		 * Calcule le delta de l'option � la date t
		 * @param[in] mod contient le mod�le de simulation
		 * @param[in] opt contient l'option
		 * @param[in]  past contient la trajectoire du sous-jacent
		 * jusqu'� l'instant t
		 * @param[in] t date � laquelle le calcul est fait
		 * @param[out]  delta contient le delta
		 * @param[out] ic contient la largeur de l'intervalle
<<<<<<< HEAD
		* de confiance sur le calcul du delta
		*/
		virtual void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic, AssetModel *mod, Option *opt, PnlRng *rng, int nbSamples) = 0;
=======
		 * de confiance sur le calcul du delta
		 */
		virtual void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic, BlackScholesModel *mod, Option *opt, PnlRng *rng, int nbSamples) = 0;
>>>>>>> 3083f9547b4241555baf613c0b52c7665b94a030
	};
}