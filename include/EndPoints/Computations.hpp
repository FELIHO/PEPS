#pragma once

#include <string>

#define DLLEXP   __declspec( dllexport )
namespace Computations {
	DLLEXP void getPriceKozeiNative(double &ic, double &prix, int n_samples,
		double h, int date, double inv_init, int numberDateEstimation, double r);
	DLLEXP std::string getDeltaKozeiNative(int n_samples,
		double h, int date, double inv_init, int numberDateEstimation, double r);
	DLLEXP std::string getListPriceKozeiNative(int n_samples,
		double h, double inv_init, int numberDateEstimation, double r);
	/*DLLEXP void getDeltaKozei(double &ic, double &prix, int n_samples,
		double h, int date, double inv_init, int numberDateEstimation, double r);
	DLLEXP void getListPriceKozei(double &ic, double &prix, int n_samples,
		double h, int date, double inv_init, int numberDateEstimation, double r);
	DLLEXP void getPnL(double &ic, double &prix, int n_samples,
		double h, int date, double inv_init, int numberDateEstimation, double r);*/
}