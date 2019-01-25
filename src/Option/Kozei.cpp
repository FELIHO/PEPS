#include "Kozei.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <algorithm>
#include <iostream>
using namespace std;



Kozei::Kozei() : Option(){
 	inv_init_ = 0.0;
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

Kozei* Kozei::clone() {
  return new Kozei(*this);
}

Kozei::Kozei( double inv_init)  {
	T_ = (double) 2080.0/260.0;
	nbTimeSteps_ = 16;
	size_ = 30;
	inv_init_ = inv_init;
}

// double Kozei::payoff(const PnlMat *path) {

// 	PnlVect *niveaux_initaux = pnl_vect_new();;
// 	pnl_mat_get_row(niveaux_initaux, path, 0);

// 	PnlVect* PerformancePanier = pnl_vect_create(nbTimeSteps_+1);
// 	double Perfmoyenne;
// 	double S_0 = 0.0;
// 	double Perf_Panier_t = 0.0;
// 	double Perf_acti = 0.0;

// 	for (int t = 0; t < nbTimeSteps_+1; t++) {
// 		for (int act = 0; act < size_; act++) {
// 			S_0 = pnl_vect_get(niveaux_initaux, act);
// 			Perf_acti = (pnl_mat_get(path, t , act) - S_0) / S_0;
// 			Perf_Panier_t += Perf_acti;
// 		}

// 		pnl_vect_set(PerformancePanier, t, std::max(Perf_Panier_t /size_,0.0) );

// 	}

// 	Perfmoyenne = pnl_vect_sum(PerformancePanier)/16;

// 	pnl_vect_free(&niveaux_initaux);
// 	pnl_vect_free(&PerformancePanier);

// 	double payoff = inv_init_ * (0.9 + Perfmoyenne);

// 	return payoff;

// }

double Kozei::payoff(const PnlMat *path) {

	PnlVect *niveaux_initaux = pnl_vect_new();
	pnl_mat_get_row(niveaux_initaux, path, 0);

	//PnlMat* Performance_t = pnl_mat_create_from_double(nbTimeSteps_+1,size_,0.0);
	PnlVect* PerformancePanier = pnl_vect_create_from_double(nbTimeSteps_+1,0.0);
	double Perfmoyenne;
    double S_0 = 0.0;
	double Perf_Panier_t = 0.0;
	double Perf_acti = 0.0;


	for (int t = 1; t < nbTimeSteps_+1; t++) {
		for (int act = 0; act < size_; act++) {
			S_0 = pnl_vect_get(niveaux_initaux, act);
			Perf_acti = (pnl_mat_get(path, t , act) - S_0) / S_0;
			//pnl_mat_set(Performance_t, t, act, Perf_acti);
			Perf_Panier_t += Perf_acti;
		}
        Perf_Panier_t = Perf_Panier_t / size_;
        pnl_vect_set(PerformancePanier, t, std::max(Perf_Panier_t, 0.0) );

    }

	Perfmoyenne = pnl_vect_sum(PerformancePanier)/16;

	//pnl_mat_print(Performance_t);
	//pnl_mat_free(&Performance_t);
	pnl_vect_free(&niveaux_initaux);
	pnl_vect_free(&PerformancePanier);


	return inv_init_ * (0.9 + Perfmoyenne);
}


