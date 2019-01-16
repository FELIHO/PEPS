#pragma once
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "DeltaCompute.hpp"

#define DLLEXP  __declspec( dllexport )
namespace Computations {
	class DeltaDF : public DeltaCompute {
	public:
		double fdStep_;
		DeltaDF(double fdStep);
		DeltaDF(const DeltaDF &B);
		~DeltaDF();
		/**
		 * Calcule le delta de l'option à la date t avec la méthode des différences finies
		 * @param[in] mod contient le modèle de simulation
		 * @param[in] opt contient l'option
		 * @param[in]  past contient la trajectoire du sous-jacent
		 * jusqu'à l'instant t
		 * @param[in] t date à laquelle le calcul est fait
		 * @param[out]  delta contient le delta
		 * @param[out] ic contient la largeur de l'intervalle
		 * de confiance sur le calcul du delta
		 */
		void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic, AssetModel *mod, Option *opt, PnlRng *rng, int nbSamples);
	};
}