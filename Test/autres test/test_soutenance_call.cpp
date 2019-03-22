#include "HedgePortfolio.hpp"
#include "Option/Basket.hpp"
#include "MonteCarlo/MonteCarlo.hpp"

#include <iostream>
#include <string>
#include "jlparser/parser.hpp"
#include <ctime>
#include <stdio.h>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{

  double T, r, strike;
  PnlVect *spot, *sigma;
  string type;
  int size;
  size_t n_samples;
  int nbTimeSteps;
  double rho;

  // Initializing Variables to contain the test results
  double price;
  double ic;

  char const *infile = "../../market-soutenance/market_call.dat";
  char const *infile1 = "../../data-soutenance/call.dat";

  Param *P = new Parser(infile1);
  PnlMat* dataHisto = pnl_mat_create_from_file(infile);

  P->extract("model type", model_type);
  P->extract("option size", size);
  P->extract("strike", strike);
  P->extract("spot", spot, size);
  P->extract("maturity", T);
  P->extract("volatility", sigma, size);
  P->extract("interest rate", r);
  P->extract("correlation", rho);
  P->extract("dividend rate", div_rate, size);

  PnlVect *weights = pnl_vect_create(size);
  PnlVect *trend = pnl_vect_create_from_scalar(size, 1);

  P->extract("option type", type);
  P->extract("payoff coefficients", weights, size);

  P->extract("Timestep number", nbTimeSteps);
  P->extract("sample number", n_samples);

  P->extract("trend", trend, size);
  P->extract("hedging dates number", nb_dates_hedge);
  P->extract("fd step", fdstep);


/* ######################################################################## */
/* ######################################################################## */

  // TEST BODY
  // #########

  // Initializing Option
  Basket *call = new Basket(T, nbTimeSteps, size, strike, weights);

  // Initializing Random Number Generator
  PnlRng* pnlRng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(pnlRng, time(NULL));
  RandomGen* rng = new PnlRand(pnlRng);

  BlackScholesModel *bs_model = new BlackScholesModel(size, r, rho, sigma, spot, trend);
  PnlMat* dataSimul = pnl_mat_new();
  bs_model->simul_market(dataSimul, T, 365, rng);

  MonteCarlo *mc_pricer = new MonteCarlo(bs_model, call, 1, n_samples);
  HedgePortfolio *hedgePortfolio = new HedgePortfolio(dataSimul, mc_pricer);

  double PL = hedgePortfolio->HedgeError(dataSimul);

  cout << endl << "##### Testing the Pricer for " << "call.dat" << " file input #####" << endl;
  cout << "#######################################################" << endl;

  std::cout<<"prix = "<<mc_pricer->price<<endl;
  std::cout<<"PL = "<<PL<<endl;
  std::cout<< "delta =" << mc_pricer->delta<<endl;

  cout << "#######################################################" << endl;

  pnl_mat_free(&dataSimul);
  delete(bs_model);
  delete(mc_pricer);
  delete(hedgePortfolio);
}
