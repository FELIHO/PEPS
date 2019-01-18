
// From "Options" Library
#include "Performance.hpp"
// From "Pricer" Library
#include "BlackScholesModel.hpp"
#include "MonteCarlo/MonteCarlo.hpp"

#include <iostream>
#include <string>
#include "jlparser/parser.hpp"
#include <ctime>

using namespace std;

int main(int argc, char **argv)
{

  double T, r;//strike // No strike in Performance Option
  PnlVect *spot, *sigma;
  string type;
  int size;
  size_t n_samples;
  int nbTimeSteps;
  double rho;

  // Initializing Variables to contain the test results
  double correct_price = 1.257353;
  double payoff = 0;
  double prix = 0;
  double ic = 0;

  char const *infile = "../../data/perf.dat";
  Param *P = new Parser(infile);

  P->extract("option type", type);
  P->extract("maturity", T);
  P->extract("option size", size);
  P->extract("timestep number", nbTimeSteps);

  // With the size, we can initialize 'path' and 'payoff coefficients'
  PnlMat *path = pnl_mat_create(nbTimeSteps + 1, size);
  PnlVect *weights = pnl_vect_create(size);

  P->extract("spot", spot, size);
  P->extract("volatility", sigma, size);
  P->extract("interest rate", r);
  //P->extract("strike", strike); // No strike in Performance Option
  P->extract("sample number", n_samples);
  P->extract("correlation", rho);
  P->extract("payoff coefficients", weights, size);

/* ######################################################################## */
/* ######################################################################## */

  // TEST BODY
  // #########

  // Initializing Option
  Performance *test_Performance = new Performance(T, nbTimeSteps, size, weights);

  // Initializing Random Number Generator
  PnlRng* pnlRng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(pnlRng, time(NULL));
  RandomGen* rng = new PnlRand(pnlRng);


  // TEST : BlackScholes
  BlackScholesModel *bs_model = new BlackScholesModel(size, r, rho, sigma, spot);
  bs_model->asset(path, T, nbTimeSteps, rng); // Simulating the path from spot t=0 only

  payoff = test_Performance->payoff(path);

  // TEST : Monte-Carlo
  MonteCarlo *mc_pricer = new MonteCarlo(bs_model, test_Performance, rng, 1, n_samples);
  mc_pricer->price(prix,ic);

  cout << endl << "##### Testing the Pricer for " << "perf.dat" << " file input #####" << endl;
  cout << "#######################################################" << endl;

  cout << "Option Payoff = " << payoff << endl;
  cout << "Price = " << prix << endl;
  cout << "Confidence Interval = " << ic << endl;

  cout << "#######################################################" << endl << endl;

/* ######################################################################## */
/* ######################################################################## */

  // FREEING DYNAMICALLY ALLOCATED VARIABLES
  // #######################################

  pnl_vect_free(&spot);
  pnl_vect_free(&sigma);
  pnl_vect_free(&weights);
  pnl_mat_free(&path);
  delete P;

  // TEST PASS VERIFICATION
  // ######################

  if ((prix >= correct_price - ic) && (prix <= correct_price + ic))
  {
    cout << "#######################################" << endl;
    cout << "# [perf.dat TEST PASSED SUCCESSFULLY] #" << endl;
    cout << "#######################################" << endl <<endl;
    exit(0);
  }
  else
  {
    cout << "##########################" << endl;
    cout << "# [perf.dat TEST FAILED] #" << endl;
    cout << "##########################" << endl << endl;
    exit(1);
  }
}
