#pragma once

#include <string>


#define DLLEXP   __declspec( dllexport )
namespace Computations {
	DLLEXP void calleuro(double &ic, double &prix, int nb_samples, double T,
		double S0, double K, double sigma, double r);
	DLLEXP void getPriceKozeiNative(double &ic, double &prix, int n_samples,
		double h, int date, double inv_init, int numberDateEstimation, double r);
	DLLEXP std::string getDeltaKozeiNative(int n_samples,
		double h, int date, double inv_init, int numberDateEstimation, double r);
	DLLEXP std::string getListPriceKozeiNative(int n_samples,
		double h, double inv_init, int numberDateEstimation, double r);
	DLLEXP void getProfitAndLossForward(double &profitAndLoss, int n_samples,
		double h, double inv_init, double r, int numberOfRebalancement, double rho, double sigma, double spot);
	DLLEXP void getProfitAndLossBackward(double &profitAndLoss, int n_samples,
		double h, double inv_init, int numberDateEstimation, double r, int numberOfRebalancement);
	DLLEXP void puteuro(double &ic, double &prix, int nb_samples, double T,
		double S0, double K, double sigma, double r);
}