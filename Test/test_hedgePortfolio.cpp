
#include "Option/Basket.hpp"
#include "HedgePortfolio.hpp"
#include "MonteCarlo/MonteCarlo.hpp"

#include <iostream>
#include <string>

#include <ctime>

using namespace std;
using namespace Computations;

int main(int argc, char **argv)
{
  double T = 1;
  int nbTimeSteps = 1;
  int size = 1;
  double r = 0.04879;
  double rho = 0;
  PnlVect *sigma = pnl_vect_create_from_double(size, 0.2);
  PnlVect *spot = pnl_vect_create_from_double(size, 3);
  PnlVect *weights = pnl_vect_create_from_double(size, 1/size);

  //FakeRnd* rng = new FakeRnd(0.3);
  PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));


  double fdStep = 1;
  int nbSamples = 50000;
  double strike = 10;

  Basket* call = new Basket(T, nbTimeSteps, size, strike, weights);
  BlackScholesModel* BMS = new BlackScholesModel(size, r, rho, sigma, spot);
  MonteCarlo* monteCarlo = new MonteCarlo(BMS, call, rng, fdStep, nbSamples);

  PnlMat* Data = pnl_mat_create_from_scalar(365, size, 100);


  cout << "test" << endl;
  HedgePortfolio *hedgePortfolio = new HedgePortfolio(Data, monteCarlo);
  double PL = hedgePortfolio->HedgeError(Data);

  cout << "prix = " << hedgePortfolio->prix_ << endl;
  cout << "P&L = " << PL << endl;

  delete(BMS);
  delete(monteCarlo);
  delete(hedgePortfolio);
  delete(rng);

}
