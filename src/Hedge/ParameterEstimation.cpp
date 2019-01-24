#include "pch.h"
#include "ParameterEstimation.hpp"
#include <math.h>
#include <algorithm>
#include <iostream>
#include <assert.h>

using namespace Computations;

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
					pnl_vect_set(logRendement, i, log(rendementJ/rendementI));
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
	assert(shareX->size == shareY->size);
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

PnlVect* ParameterEstimation::getTrend(const PnlMat *path) {
	return pnl_vect_create_from_double(path->n, 0.3);
}
