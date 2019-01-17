#include <iostream>
#include "pch.h"
using namespace std;
using namespace Computations;
int main()
{
	cout << "Hello, World!";
	PnlMat *path = pnl_mat_new();
	//path = pnl_mat_create_from_file("C:/Users/ensimag/Desktop/PEPS/Data/market_basket.dat");
	ParameterEstimation p;
	PnlVect* volVector = p.getVolatilitiesVector(path);
	pnl_vect_print(volVector);
	return 0;
}