
#include "Basket.hpp"
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"

#include <iostream>
#include <string>
#include "jlparser/parser.hpp"
#include <ctime>

using namespace std;
using namespace Computations;

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
  double payoff = 0;
  double prix = 0;
  double ic = 0;

  char const *infile = "../../data/basket.dat";
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
  P->extract("strike", strike);
  P->extract("sample number", n_samples);
  P->extract("correlation", rho);
  P->extract("payoff coefficients", weights, size);


/* ######################################################################## */
/* ######################################################################## */

  // TEST BODY
  // #########

  // Initializing Option
  Basket *test_Basket = new Basket(T, nbTimeSteps, size, strike, weights);

  // Initializing Random Number Generator
  PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));
  //RandomGen* rng = new PnlRand(pnlRng);


  // TEST : BlackScholes
  BlackScholesModel *bs_model = new BlackScholesModel(size, r, 0.0, sigma, spot);
  bs_model->asset(path, T, nbTimeSteps, rng); // Simulating the path from spot t=0 only


  cout << "### Testing BlackScholesModel for " << "basket.dat" << " file input" << endl;
  cout << "#######################################################" << endl;

  cout << "path matrix size = " << path->n << endl;
  cout << "path matrix nbTimeSteps = " << path->m << endl;
  cout << "Printing the path matrix :" << endl << endl;

  pnl_mat_print(path);

  cout << "#######################################################" << endl;



/* ######################################################################## */
/* ######################################################################## */

  // FREEING DYNAMICALLY ALLOCATED VARIABLES
  // #######################################

  pnl_vect_free(&spot);
  pnl_vect_free(&sigma);
  pnl_vect_free(&weights);
  pnl_mat_free(&path);
  delete P;
  delete(rng);

}
