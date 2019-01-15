#include <assert.h>
#include <sstream>
#include <iostream>
#include <ctime>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_finance.h"
#include "../src/Basket.hpp"
#include "../src/Basket.cpp"
#include "../src/BlackScholesModel.hpp"
#include "../src/BlackScholesModel.cpp"
#include "../src/MonteCarlo.cpp"
using namespace std;

int main(){
  double s = 100.0;
  double k = 110.0;
  double T = 3.0;
  double r = 0.05;
  double divid = 0;
  double sigma = 0.2;
  int size = 1;
  int nbTimeStep = 1000;
  int nbSamples = 1E5;
  double rho = 1.0;

  PnlVect *sigmaVect = pnl_vect_create_from_scalar(size,sigma);
  PnlVect *spotVect = pnl_vect_create_from_scalar(size,s);
  PnlVect *payOffCoeffVect = pnl_vect_create_from_scalar(size,1.0);
  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);

  Basket *BasketTest = new Basket(T, nbTimeStep , size, k, payOffCoeffVect);
  BlackScholesModel *BSMTest = new BlackScholesModel(size , r, rho, sigmaVect, spotVect);
  MonteCarlo MCTest = MonteCarlo(BSMTest, BasketTest, nbSamples, rng, 0.001);

  double prix = 0.0;
  double ic = 0.5;
  MCTest.price(prix, ic);

  cout << "Prix B&M : "<< pnl_bs_call (s, k, T, r, divid, sigma) << endl;
  cout << "Prix MC : "<< prix << endl;

  pnl_rng_free(&rng);
  pnl_vect_free(&sigmaVect);
  pnl_vect_free(&spotVect);
  pnl_vect_free(&payOffCoeffVect);
}
