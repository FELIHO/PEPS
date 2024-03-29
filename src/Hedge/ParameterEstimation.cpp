#include "ParameterEstimation.hpp"

PnlMat* ParameterEstimation::getLogRendementMatrix(const PnlMat *past) {
	PnlMat* logRendementMatrix = pnl_mat_create(past->m - 1, past-> n);
	PnlVect* rendement = pnl_vect_new();
	PnlVect* Nextrendement = pnl_vect_new();
	PnlVect* logRendement = pnl_vect_create(past->n);
	double rendementI;
	double rendementJ;
	for (int j = 0; j < logRendementMatrix->m; j++) {
		pnl_mat_get_row(Nextrendement, past, j + 1);
		pnl_mat_get_row(rendement, past, j);
		for (int i = 0; i < logRendementMatrix->n; i++){
				rendementI = pnl_vect_get(rendement, i);
				rendementJ = pnl_vect_get(Nextrendement, i);
				if ((rendementI < 0.001) || (rendementJ < 0.001)){
					pnl_vect_set(logRendement, i, 0.0);
				} else {
					pnl_vect_set(logRendement, i, log(rendementJ/rendementI)/sqrt(1./Tools::NumberOfDaysPerYear));
				}
			}
			pnl_mat_set_row(logRendementMatrix, logRendement, j);
	}

	pnl_vect_free(&rendement);
	pnl_vect_free(&logRendement);
	pnl_vect_free(&Nextrendement);
	return logRendementMatrix;
}


PnlMat* ParameterEstimation::getCovarianceMatrix(const PnlMat *past) {
	PnlMat* logRendement = getLogRendementMatrix(past);
	PnlMat* covMatrix = pnl_mat_create(logRendement->n, logRendement->n);
	PnlVect* X = pnl_vect_new();
	PnlVect* Y = pnl_vect_new();
	double E_X;
	double E_Y;
	double E_XY;
	double cov;
	int nbDate = logRendement->m;
	for (int i = 0; i < logRendement->n; i++) {
		for (int j = 0; j <= i; j++) {
			pnl_mat_get_col(X, logRendement, i);
			pnl_mat_get_col(Y, logRendement, j);
			E_X = pnl_vect_sum(X)/ nbDate;
			E_Y = pnl_vect_sum(Y) / nbDate;
			pnl_vect_mult_vect_term(X, Y);
			E_XY = pnl_vect_sum(X) / nbDate;
			cov = E_XY - E_X * E_Y ;
			pnl_mat_set(covMatrix, i, j, cov);
			pnl_mat_set(covMatrix, j, i, cov);
		}
	}

	pnl_vect_free(&X);
	pnl_vect_free(&Y);
	pnl_mat_free(&logRendement);
	return covMatrix;

}

PnlMat* ParameterEstimation::getCorrelationMatrix(const PnlMat *past) {
	PnlMat* covMatrix = getCovarianceMatrix(past);
	PnlMat* corrMatrix = pnl_mat_create_diag(pnl_vect_create_from_scalar(covMatrix->n, 1));
	double Sigma_X;
	double Sigma_Y;
	double cor;
	for (int i = 0; i < covMatrix->n; i++) {
		for (int j = 0; j < i; j++) {
			Sigma_X = sqrt(pnl_mat_get(covMatrix, i, i));
			Sigma_Y = sqrt(pnl_mat_get(covMatrix, j, j));
			cor = pnl_mat_get(covMatrix, i, j) / (Sigma_X * Sigma_Y);
			pnl_mat_set(corrMatrix, i, j, cor);
			pnl_mat_set(corrMatrix, j, i, cor);

		}
	}
	pnl_mat_free(&covMatrix);
	return corrMatrix;
}

double ParameterEstimation::getSigmaCorreled(const double Sigma_X, const double Sigma_Y, const double rhoXY) {
	return sqrt(Sigma_X*Sigma_X + Sigma_Y*Sigma_Y + Sigma_X*Sigma_Y*rhoXY);
}

double ParameterEstimation::getCorrelation(const PnlVect* shareX, const PnlVect* shareY) {
	//assert(shareX->size == shareY->size);
	if(shareX->size != shareY->size){
    	throw new length_error("Les matrices shareX et shareY ne sont pas de la même taille") ;
 	}
	PnlMat* corrMatrix = pnl_mat_create(shareX->size, 2);
	pnl_mat_set_col (corrMatrix, shareX, 0);
	pnl_mat_set_col (corrMatrix, shareY, 1);
	corrMatrix = getCorrelationMatrix(corrMatrix);
	double corr = pnl_mat_get(corrMatrix, 0, 1);
	pnl_mat_free(&corrMatrix);
	return corr;
}

PnlVect* ParameterEstimation::getVolatilitiesVector(const PnlMat *path) {
	PnlMat* covMatrix = getCovarianceMatrix(path);
	PnlVect* volatilitiesVector = pnl_vect_create_from_scalar(covMatrix->n, 0.0);
	for (int j = 0; j < covMatrix->n; j++) {
		pnl_vect_set(volatilitiesVector, j, sqrt(pnl_mat_get(covMatrix, j, j)));
	}
	return volatilitiesVector;
}

PnlMat* ParameterEstimation::getDomesticAssetPrices(const PnlMat *pricesforeigncurr,const PnlMat* exchangerates){
	PnlMat* Domesticprices =  pnl_mat_copy(pricesforeigncurr);
	PnlVect* Asset = pnl_vect_create(pricesforeigncurr->m);
	PnlVect* USD_Exchange_Rates = pnl_vect_create(exchangerates->m);
	pnl_mat_get_col(USD_Exchange_Rates,exchangerates,0);
	for(int s = 0;s<=16;s++ ){

		pnl_mat_get_col(Asset,pricesforeigncurr,s);
		pnl_vect_mult_vect_term (Asset, USD_Exchange_Rates);
		pnl_mat_set_col(Domesticprices,Asset,s);

	}

	PnlVect* JPY_Exchange_Rates = pnl_vect_create(exchangerates->m);
	pnl_mat_get_col(JPY_Exchange_Rates,exchangerates,1);
	for(int s = 17;s<=18;s++ ){
		pnl_mat_get_col(Asset,pricesforeigncurr,s);
		pnl_vect_mult_vect_term (Asset, JPY_Exchange_Rates);
		pnl_mat_set_col(Domesticprices,Asset,s);

	}

	PnlVect* GBP_Exchange_Rates = pnl_vect_create(exchangerates->m);
	pnl_mat_get_col(GBP_Exchange_Rates,exchangerates,2);
	for(int s = 19;s<=20;s++ ){
		pnl_mat_get_col(Asset,pricesforeigncurr,s);
		pnl_vect_mult_vect_term (Asset, GBP_Exchange_Rates);
		pnl_mat_set_col(Domesticprices,Asset,s);

	}

	PnlVect* CHF_Exchange_Rates = pnl_vect_create(exchangerates->m);
	pnl_mat_get_col(CHF_Exchange_Rates,exchangerates,3);
	pnl_mat_get_col(Asset,pricesforeigncurr,28);
	pnl_vect_mult_vect_term (Asset, CHF_Exchange_Rates);
	pnl_mat_set_col(Domesticprices,Asset,28);

	PnlVect* BRL_Exchange_Rates = pnl_vect_create(exchangerates->m);
	pnl_mat_get_col(BRL_Exchange_Rates,exchangerates,4);
	pnl_mat_get_col(Asset,pricesforeigncurr,29);
	pnl_vect_mult_vect_term (Asset, BRL_Exchange_Rates);
	pnl_mat_set_col(Domesticprices,Asset,29);

	pnl_vect_free(&USD_Exchange_Rates);
	pnl_vect_free(&JPY_Exchange_Rates);
	pnl_vect_free(&GBP_Exchange_Rates);
	pnl_vect_free(&CHF_Exchange_Rates);
	pnl_vect_free(&BRL_Exchange_Rates);
	pnl_vect_free(&Asset);

	return Domesticprices;
}

PnlVect* ParameterEstimation::getTrend(const PnlMat *path){
	PnlVect* TrendVector = pnl_vect_create(path->n);
	PnlMat* logrendementMatrix = getLogRendementMatrix(path);
	PnlVect* logrendementVect_s = pnl_vect_create(logrendementMatrix->m);
	double trend = 0.0;
	for(int s=0;s<path->n;s++){
		pnl_mat_get_col(logrendementVect_s,logrendementMatrix,s);
		trend = pnl_vect_sum(logrendementVect_s)/logrendementVect_s->size;
		pnl_vect_set(TrendVector,s,trend);
	}
	pnl_mat_free(&logrendementMatrix);
	pnl_vect_free(&logrendementVect_s);

	return TrendVector;
}
