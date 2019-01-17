#include "pch.h"
#include "Kozei.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <algorithm>
#include "Option.hpp"
using namespace Computations;



Kozei::Kozei() :Option() {
 	inv_init_ = 0.0;
}

Kozei::Kozei( double inv_init)  {
	inv_init_ = inv_init;	
}

Kozei::Kozei(const Kozei &K) {
	T_ = K.T_;
	nbTimeSteps_ = K.nbTimeSteps_;
	size_ = K.size_;
	inv_init_ = K.inv_init_;
}

Kozei& Kozei::operator=(const Kozei&K) {
	T_ = K.T_;
	nbTimeSteps_ = K.nbTimeSteps_;
	size_ = K.size_;
	inv_init_ = K.inv_init_;
	return *this;
}

Kozei::~Kozei() {
}

double Kozei::payoff(const PnlMat *path) {
	/* A revoir certains entier sont pris comme des double par rapport au temps.
	* Il faut convertir la valeur temporelle en double � l'entier qui lui correponds dans la matrice
	* en gros, t est entre tk et tk+1, il faut l'op�ration qui pour t donne le k.
	*/
	PnlVect *niveaux_initaux = pnl_vect_create(size_);
	PnlMat* Performance_t = pnl_mat_create(T_ * 2,size_);
	PnlVect* PerformancePanier = pnl_vect_create(T_ * 2);
	double Perfmoyenne;

	for (int i = 0; i < size_; i++) {
		pnl_vect_set(niveaux_initaux,i,  1 / 3 * (pnl_mat_get(path, 0, i) + pnl_mat_get(path, 1, i) + pnl_mat_get(path, 2, i)) );
	}

	for (int t = 0; t < T_ * 2; t++) {
		double S_0 = 0.0;
		double Perf_Panier_t = 0.0;
		double Perf_acti = 0.0;
		for (int act = 0; act < size_; act++) {
			S_0 = pnl_vect_get(niveaux_initaux, act);
			Perf_acti = (pnl_mat_get(path, t + 3, act) - S_0) / S_0;
			pnl_mat_set(Performance_t, t, act, Perf_acti);
			Perf_Panier_t += Perf_acti;
		}

		pnl_vect_set(PerformancePanier, t, std::max(Perf_Panier_t /size_,0.0) );
	}

	Perfmoyenne = pnl_vect_sum(PerformancePanier)/16;
	
	free(&niveaux_initaux);
	free(&Performance_t);
	free(&PerformancePanier);


	return inv_init_ * (0.9 + Perfmoyenne);


	
}