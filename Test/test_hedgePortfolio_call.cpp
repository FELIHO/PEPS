#include "HedgePortfolio.hpp"
#include "Basket.hpp"
#include "MonteCarlo.hpp"

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
  double correct_price = 13.616294;
  //double payoff = 0;
  double prix;
  double ic;

  char const *infile = "../../market-data/simul_call.dat";
  char const *infile1 = "../../produits/call.dat";

  Param *P = new Parser(infile1);
  PnlMat* dataHisto = pnl_mat_create_from_file(infile);

  P->extract("option type", type);
  P->extract("maturity", T);
  P->extract("option size", size);
  P->extract("timestep number", nbTimeSteps);

  //PnlMat *path = pnl_mat_create(nbTimeSteps + 1, size);
  PnlVect *weights = pnl_vect_create(size);
  PnlVect *trend = pnl_vect_create_from_scalar(size, 1);


  P->extract("spot", spot, size);
  P->extract("volatility", sigma, size);
  P->extract("interest rate", r);
  P->extract("strike", strike);
  P->extract("sample number", n_samples);
  P->extract("correlation", rho);
  P->extract("payoff coefficients", weights, size);
  P->extract("trend", trend, size);


/* ######################################################################## */
/* ######################################################################## */

  // TEST BODY
  // #########

  // Initializing Option
  Basket *call = new Basket(T, nbTimeSteps, size, strike, weights);

  // Initializing Random Number Generator
  PnlRng* pnlRng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(pnlRng, time(NULL));
  RandomGen* rng = new PnlRnd(pnlRng);


  BlackScholesModel *bs_model = new BlackScholesModel(size, r, rho, sigma, spot, trend);
  PnlMat* dataSimul = pnl_mat_new();
  pnl_vect_print(spot);
  bs_model->simul_market(dataSimul, T, 365, rng);

  MonteCarlo *mc_pricer = new MonteCarlo(bs_model, call, rng, 1, n_samples);
  HedgePortfolio *hedgePortfolio = new HedgePortfolio(dataSimul, mc_pricer);

  double PL = hedgePortfolio->HedgeError(dataSimul);

  cout << endl << "##### Testing the Pricer for " << "test.hedgePortfolio_call" << " file input #####" << endl;
  cout << "#######################################################" << endl;

  std::cout<<"prix = "<<hedgePortfolio->prix_<<endl;
  std::cout<<"PL = "<<PL<<endl;

  cout << "#######################################################" << endl;

  pnl_mat_free(&dataSimul);
  delete(bs_model);
  delete(mc_pricer);
  delete(hedgePortfolio);
}
