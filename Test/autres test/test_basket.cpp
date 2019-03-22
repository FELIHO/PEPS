#include "Basket.hpp"
#include "MonteCarlo.hpp"

#include <iostream>
#include <string>
#include "jlparser/parser.hpp"
#include <ctime>

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
  double correct_price = 13.627;
  double sd = 0.025;
  //double payoff = 0;
  double prix;
  double ic;

  char const *infile = "../../data/basket.dat";
  Param *P = new Parser(infile);

  P->extract("option type", type);
  P->extract("maturity", T);
  P->extract("option size", size);
  P->extract("timestep number", nbTimeSteps);

  //PnlMat *path = pnl_mat_create(nbTimeSteps + 1, size);
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


  BlackScholesModel *bs_model = new BlackScholesModel(size, r, rho, sigma, spot);


  MonteCarlo *mc_pricer = new MonteCarlo(bs_model, test_Basket, 1, n_samples);
  mc_pricer->price(prix,ic);

  // Initializing the past with the spot
  PnlMat* past = pnl_mat_create(1, bs_model->size_);
  pnl_mat_set_row(past, bs_model->spot_, 0);

  // delta at t=0
  PnlVect* delta = pnl_vect_new();
  mc_pricer->delta(past, 0, delta);

  cout << endl << "### Testing the Pricer for " << "basket.dat" << " file input ###" << endl << endl;
  cout << "#######################################################" << endl;

  // cout << "Option Payoff = " << payoff << endl;
  cout << "Price = " << prix << endl;
  cout << "Confidence Interval = " << ic << endl;

  cout << "#######################################################"<< endl << endl;

/* ######################################################################## */
/* ######################################################################## */

  // FREEING DYNAMICALLY ALLOCATED VARIABLES
  // #######################################

  // pnl_mat_free(&path);
  pnl_vect_free(&delta);
  delete(bs_model);
  // delete(mc_pricer);
  delete P;
  delete(test_Basket);

  // TEST PASS VERIFICATION
  // ######################

  assert(correct_price >= prix - ic);
  assert(correct_price <= prix + ic);
  assert(prix >= correct_price - sd*2);
  assert(prix <= correct_price + sd*2);

  cout << "# [test_basket TEST PASSED SUCCESSFULLY] #" << endl << endl;

}
