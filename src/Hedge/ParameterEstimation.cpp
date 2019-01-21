#include "pch.h"
#include "ParameterEstimation.hpp"
#include <math.h>

using namespace std;
using namespace Computations;

PnlMat* ParameterEstimation::getLogRendementMatrix(const PnlMat *past) {
	PnlMat* logRendementMatrix = pnl_mat_create(past->m - 1, past-> n);
	PnlVect* rendement = pnl_vect_new();
	PnlVect* Nextrendement = pnl_vect_new();
	for (int j = 0; j < logRendementMatrix->m; j++) {
		pnl_mat_get_row(Nextrendement, past, j + 1);
		pnl_mat_get_row(rendement, past, j);
		pnl_vect_div_vect_term(Nextrendement, rendement);
		pnl_vect_map_inplace(Nextrendement, &makeLogonAllElements);
		pnl_mat_set_row(logRendementMatrix, Nextrendement, j);
	}
	pnl_vect_free(&rendement);
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
			cov = E_XY - E_X *E_Y ;
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
			Sigma_X = sqrt(abs((long)pnl_mat_get(covMatrix, i, i)));
			Sigma_Y = sqrt(abs((long)pnl_mat_get(covMatrix, j, j)));
			cor = pnl_mat_get(covMatrix, i, j) / (Sigma_X * Sigma_Y);
			pnl_mat_set(corrMatrix, i, j, cor);
			pnl_mat_set(corrMatrix, j, i, cor);

		}
	}
	pnl_mat_free(&covMatrix);
	return corrMatrix;
}

PnlVect* ParameterEstimation::getVolatilitiesVector(const PnlMat *past) {
	PnlMat* covMatrix = getCovarianceMatrix(past);
	PnlVect* volatilities = pnl_vect_create(covMatrix->n);
	int numberOfDaysPerYear = 252;
	for (int i = 0; i < covMatrix->n; i++) {
		pnl_vect_set(volatilities, i, sqrt(pnl_mat_get(covMatrix, i, i) * numberOfDaysPerYear));
	}
	pnl_mat_free(&covMatrix);
	return volatilities;
}

double makeLogonAllElements(double vectorElement) {
	return log(vectorElement);
}
