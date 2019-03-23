#pragma once

#include <string>
#include <iostream>
#include <time.h>
#include "pnl/pnl_random.h"
#include "MonteCarlo.hpp"
#include "Kozei.hpp"
#include "ParameterEstimation.hpp"
#include "DataSelecter.hpp"
#include <list>

#define DLLEXP   __declspec( dllexport )
namespace Computations {
	/*DLLEXP*/ void calleuro(double &ic, double &prix, int nb_samples, double T,
		double S0, double K, double sigma, double r);
	/*DLLEXP*/ void getPriceKozeiNative(double &ic, double &prix, int n_samples,
		double h, int date, double inv_init, int numberDateEstimation, double r);
	/*DLLEXP*/ std::string getDeltaKozeiNative(int n_samples,
		double h, int date, double inv_init, int numberDateEstimation, double r);
	/*DLLEXP*/ std::string getListPriceKozeiNative(int n_samples,
		double h, double inv_init, int numberDateEstimation, double r);
}