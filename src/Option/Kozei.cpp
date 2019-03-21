#include "Kozei.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Tools.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
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


double Kozei::payoff_with_ExR(const PnlMat *path) {

	PnlMat* assets_path = pnl_mat_create(size_,path->n);

	pnl_mat_extract_subblock (assets_path, path, 0,path->m, 0,size_);

	PnlVect *niveaux_initaux_assets = pnl_vect_new();
	pnl_mat_get_row(niveaux_initaux_assets, assets_path, 0);

	PnlMat* ExR_path = pnl_mat_create(5,path->n);
	pnl_mat_extract_subblock (ExR_path, path, 0,path->m, size_+1,path->n);

	PnlVect *niveaux_initaux_ExR = pnl_vect_new();
	pnl_mat_get_row(niveaux_initaux_ExR, ExR_path, 0);

	//PnlMat* Performance_t = pnl_mat_create_from_double(nbTimeSteps_+1,size_,0.0);
	PnlVect* PerformancePanier = pnl_vect_create_from_double(nbTimeSteps_+1,0.0);
	double Perfmoyenne;
    double S_0 ,S_t,B_0X_0,B_tX_t,B_t,B_0;
	double Perf_Panier_t = 0.0;
	double Perf_acti = 0.0;

	/*
	L ordre des actifs dans la matrice Path
	USD	JPY	GBP	EUR	CHF	BRL
	17	2	2	7	1	1
	*/

	for (int t = 1; t < nbTimeSteps_+1; t++) {

		// Calcule de Performance des actifs en USD
		for (int act = 0; act < 17; act++) {
			B_0 = exp(-r_USD*T_);
			B_t = exp(-r_USD*(T_-t));
			S_0 = pnl_vect_get(niveaux_initaux_assets, act);
			S_t = pnl_mat_get(assets_path, t , act);
			B_0X_0 = pnl_vect_get(niveaux_initaux_ExR,0);
			B_tX_t = pnl_mat_get(ExR_path, t , 0);
			Perf_acti = (S_t *B_0X_0*B_t) / (S_0*B_tX_t*B_0);
			//pnl_mat_set(Performance_t, t, act, Perf_acti);
			Perf_Panier_t += Perf_acti;
		}
		// Calcule de Performance des actifs en JPY
		for (int act = 17; act < 19 ; act++) {
			B_0 = exp(-r_JPY*T_);
			B_t = exp(-r_JPY*(T_-t));
			S_0 = pnl_vect_get(niveaux_initaux_assets, act);
			S_t = pnl_mat_get(assets_path, t , act);
			B_0X_0 = pnl_vect_get(niveaux_initaux_ExR,1);
			B_tX_t = pnl_mat_get(ExR_path, t , 1);
			Perf_acti = (S_t *B_0X_0*B_t) / (S_0*B_tX_t*B_0)-1;
			//pnl_mat_set(Performance_t, t, act, Perf_acti);
			Perf_Panier_t += Perf_acti;
		}

		// Calcule de Performance des actifs en GBP
		for (int act = 19; act < 21  ; act++) {
			B_0 = exp(-r_GBP*T_);
			B_t = exp(-r_GBP*(T_-t));
			S_0 = pnl_vect_get(niveaux_initaux_assets, act);
			S_t = pnl_mat_get(assets_path, t , act);
			B_0X_0 = pnl_vect_get(niveaux_initaux_ExR,2);
			B_tX_t = pnl_mat_get(ExR_path, t , 2);
			Perf_acti = (S_t *B_0X_0*B_t) / (S_0*B_tX_t*B_0) -1;
			//pnl_mat_set(Performance_t, t, act, Perf_acti);
			Perf_Panier_t += Perf_acti;
		}

		// Calcule de Performance des actifs en EUR
		for (int act = 21; act < 28  ; act++) {
			S_0 = pnl_vect_get(niveaux_initaux_assets, act);
			S_t = pnl_mat_get(assets_path, t , act);
			B_0X_0 = pnl_vect_get(niveaux_initaux_ExR,2);
			B_tX_t = pnl_mat_get(ExR_path, t , 2);
			Perf_acti = (S_t )/ (S_0) - 1;
			//pnl_mat_set(Performance_t, t, act, Perf_acti);
			Perf_Panier_t += Perf_acti;
		}

		// Calcule de Performance des actifs en CHF
		for (int act = 28; act < 29  ; act++) {
			B_0 = exp(-r_CHF*T_);
			B_t = exp(-r_CHF*(T_-t));
			S_0 = pnl_vect_get(niveaux_initaux_assets, act);
			S_t = pnl_mat_get(assets_path, t , act);
			B_0X_0 = pnl_vect_get(niveaux_initaux_ExR,3);
			B_tX_t = pnl_mat_get(ExR_path, t , 3);
			Perf_acti = (S_t *B_0X_0*B_t) / (S_0*B_tX_t*B_0) -1;
			//pnl_mat_set(Performance_t, t, act, Perf_acti);
			Perf_Panier_t += Perf_acti;
		}

		// Calcule de Performance des actifs en BRL
		for (int act = 29; act < 30  ; act++) {
			B_0 = exp(-r_BRL*T_);
			B_t = exp(-r_BRL*(T_-t));
			S_0 = pnl_vect_get(niveaux_initaux_assets, act);
			S_t = pnl_mat_get(assets_path, t , act);
			B_0X_0 = pnl_vect_get(niveaux_initaux_ExR,4);
			B_tX_t = pnl_mat_get(ExR_path, t , 4);
			Perf_acti = (S_t *B_0X_0*B_t) / (S_0*B_tX_t*B_0) -1;
			//pnl_mat_set(Performance_t, t, act, Perf_acti);
			Perf_Panier_t += Perf_acti;
		}

        Perf_Panier_t = Perf_Panier_t / size_;
        pnl_vect_set(PerformancePanier, t, std::max(Perf_Panier_t, 0.0) );

    }

	Perfmoyenne = pnl_vect_sum(PerformancePanier)/16;

	//pnl_mat_print(Performance_t);
	//pnl_mat_free(&Performance_t);
	pnl_mat_free(&assets_path);
	pnl_mat_free(&ExR_path);	
	pnl_vect_free(&niveaux_initaux_assets);
	pnl_vect_free(&niveaux_initaux_ExR);
	pnl_vect_free(&PerformancePanier);


	return inv_init_ * (0.9 + Perfmoyenne);
}

PnlVect* Kozei::Foreign_ZC(int number_of_dates,double rate){
	PnlVect* Fr_ZC = pnl_vect_create(number_of_dates);
	double t = 0; 
	for(int i = 0 ;i<number_of_dates;i++){
		pnl_vect_set(Fr_ZC,i,exp(-rate*(T_-t)));
		t+=1./Tools::NumberOfDaysPerYear;
	}
	return Fr_ZC;
}

PnlMat* Kozei::return_path_matrix(PnlMat *const assets_path,const PnlMat* ExR_path){
	/*
	L ordre des actifs dans la matrice assets Path
	USD	JPY	GBP	EUR	CHF	BRL
	17	2	2	7	1	1
	*/
	/*
	L ordre des taux de change dans la matrice ExR path
	USD JPY GBP CHF BRL
	*/
	PnlVect* USD_ExR ;
	PnlVect* JPY_ExR;
	PnlVect* GBP_ExR;
	PnlVect* CHF_ExR;
	PnlVect* BRL_ExR;
	PnlVect* asset;
	if(assets_path-> m != ExR_path->m){
		throw length_error("la matrice des actifs et celle des taux de changes doivent avoir le même nombre de lignes") ;
	}
	PnlMat* Path = pnl_mat_create (assets_path-> m,assets_path-> n + ExR_path-> n);
	pnl_mat_get_col (USD_ExR, ExR_path, 0);
	pnl_mat_get_col (JPY_ExR, ExR_path, 1);
	pnl_mat_get_col (GBP_ExR, ExR_path, 2);
	pnl_mat_get_col (CHF_ExR, ExR_path, 3);
	pnl_mat_get_col (BRL_ExR, ExR_path, 4);
	
	for(int i = 0; i<assets_path-> n + ExR_path-> n;i++){
		if(i>=0 && i<17){
			pnl_mat_get_col (asset, assets_path, i);
			pnl_vect_mult_vect_term (asset, USD_ExR);
			pnl_mat_set_col (Path, asset,i);
		}
		if(i>=17 && i<19){
			pnl_mat_get_col (asset, assets_path, i);
			pnl_vect_mult_vect_term (asset, JPY_ExR);
			pnl_mat_set_col (Path, asset,i);
		}
		if(i>=19 && i<21){
			pnl_mat_get_col (asset, assets_path, i);
			pnl_vect_mult_vect_term (asset, GBP_ExR);
			pnl_mat_set_col (Path, asset,i);
		}
		if(i==28){
			pnl_mat_get_col (asset, assets_path, i);
			pnl_vect_mult_vect_term (asset, CHF_ExR);
			pnl_mat_set_col (Path, asset,i);
		}
		if(i==29){
			pnl_mat_get_col (asset, assets_path, i);
			pnl_vect_mult_vect_term (asset, BRL_ExR);
			pnl_mat_set_col (Path, asset,i);
		}
		// X_USD*B_USD(t,T)
		if(i == 30){
			pnl_mat_get_col (asset, ExR_path, 0);
			pnl_vect_mult_vect_term (asset,Foreign_ZC(ExR_path->m,r_USD));
			pnl_mat_set_col (Path, asset,i);		
		}
		// X_JPY*B_JPY(t,T)
		if(i == 31){
			pnl_mat_get_col (asset, ExR_path, 1);
			pnl_vect_mult_vect_term (asset,Foreign_ZC(ExR_path->m,r_JPY));
			pnl_mat_set_col (Path, asset,i);		
		}
		// X_GBP*B_GBP(t,T)
		if(i == 32){
			pnl_mat_get_col (asset, ExR_path, 2);
			pnl_vect_mult_vect_term (asset,Foreign_ZC(ExR_path->m,r_GBP));
			pnl_mat_set_col (Path, asset,i);		
		}
		// X_CHF*B_CHF(t,T)
		if(i == 33){
			pnl_mat_get_col (asset, ExR_path, 3);
			pnl_vect_mult_vect_term (asset,Foreign_ZC(ExR_path->m,r_CHF));
			pnl_mat_set_col (Path, asset,i);		
		}
		// X_BRL*B_BRL(t,T)
		if(i == 34){
			pnl_mat_get_col (asset, ExR_path, 4);
			pnl_vect_mult_vect_term (asset,Foreign_ZC(ExR_path->m,r_BRL));
			pnl_mat_set_col (Path, asset,i);		
		}
	}
		pnl_vect_free(&USD_ExR);
		pnl_vect_free(&JPY_ExR);
		pnl_vect_free(&GBP_ExR);
		pnl_vect_free(&CHF_ExR);
		pnl_vect_free(&BRL_ExR);
		pnl_vect_free(&asset);

}

