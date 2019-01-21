#include "Option/Basket.hpp"
#include "MonteCarlo/MonteCarlo.hpp"

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
  double correct_price = 9.238710;
  // double payoff = 0;
  double prix;
  double ic;

  char const *infile = "../../data/basket_2.dat";
  Param *P = new Parser(infile);

  P->extract("option type", type);
  P->extract("maturity", T);
  P->extract("option size", size);
  P->extract("timestep number", nbTimeSteps);

  // PnlMat *path = pnl_mat_create(nbTimeSteps + 1, size);
  PnlVect *weights = pnl_vect_create(size);

  P->extract("spot", spot, size);
  P->extract("volatility", sigma, size);
  P->extract("interest rate", r);
  P->extract("strike", strike);
  P->extract("sample number", n_samples);
  P->extract("correlation", rho);
  P->extract("payoff coefficients", weights, size);

  PnlVect* deltas_correct = pnl_vect_create_from_list(size, 0.0162094, 0.01621, 0.01622, 0.0162203, 0.0162145, 0.0162044, 0.016215, 0.0162163, 0.0161995, 0.0161993, 0.0162085, 0.0161999, 0.0161848, 0.0161824, 0.0162018, 0.0161994, 0.0161971, 0.0162066, 0.0162119, 0.0162086, 0.0162088, 0.0162014, 0.0162149, 0.0162236, 0.0162311, 0.0162138, 0.0162064, 0.0162129, 0.0162166, 0.0162183, 0.0161925, 0.0162276, 0.016189, 0.0161958, 0.0162017, 0.0162142, 0.0162028, 0.0162092, 0.0162104, 0.0162025);

/* ######################################################################## */
/* ######################################################################## */

  // TEST BODY
  // #########

  // Initializing Option
  Basket *test_Basket_2 = new Basket(T, nbTimeSteps, size, strike, weights);

  // Initializing Random Number Generator
  PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));

  BlackScholesModel *bs_model = new BlackScholesModel(size, r, rho, sigma, spot);
  // bs_model->asset(path, T, nbTimeSteps, rng); // Simulating the path from spot t=0 only
  // payoff = test_Basket_2->payoff(path);

  MonteCarlo *mc_pricer = new MonteCarlo(bs_model, test_Basket_2, rng, 1, n_samples);
  mc_pricer->price(prix, ic);

  // Initializing the past with the spot
  PnlMat* past = pnl_mat_create(1, bs_model->size_);
  pnl_mat_set_row(past, bs_model->spot_, 0);

  // delta at t=0
  PnlVect* delta = pnl_vect_new();
  mc_pricer->delta(past, 0, delta);

  // relative_error_vect : vector filled with the relative error between the correct deltas and the calculated deltas
  // mean ratio : mean of the relative error
  // relative_error_bool : boolean that checks if the relative error is not up to 10%
  bool relative_error_bool = true;
  double relative_error_mean = 0;
  PnlVect* relative_error_vect = pnl_vect_create(size);

  for (int i = 0; i < size; i++) {
    double ref = pnl_vect_get(deltas_correct, i);
    double diff_i = abs((pnl_vect_get(delta, i) - ref))/ref;
    relative_error_mean += diff_i;
    pnl_vect_set(relative_error_vect, i, diff_i);
    if (diff_i > 0.1) {
      relative_error_bool = false;
    }
  }
  relative_error_mean =  relative_error_mean/size;

  cout << endl << "### Testing the Pricer for " << "basket_2.dat" << " file input ###" << endl << endl;
  cout << "#######################################################" << endl;

  // cout << "Option Payoff = " << payoff << endl;
  cout << "Price = " << prix << endl;
  cout << "Confidence Interval = " << ic << endl;
  cout<< "Mean of the relative error on the deltas = " << relative_error_mean  << endl;

  cout << "#######################################################" << endl << endl;

/* ######################################################################## */
/* ######################################################################## */

  // FREEING DYNAMICALLY ALLOCATED VARIABLES
  // #######################################

  // pnl_mat_free(&path);
  pnl_vect_free(&deltas_correct);
  pnl_mat_free(&past);
  pnl_vect_free(&delta);
  pnl_vect_free(&relative_error_vect);

  delete(bs_model);
  // delete(mc_pricer);
  delete(rng);
  delete(P);
  delete(test_Basket_2);

  // TEST PASS VERIFICATION
  // ######################

  assert(correct_price >= prix - ic);
  assert(correct_price <= prix + ic);
  assert(relative_error_bool = true);

  cout << "# [test_basket_2 TEST PASSED SUCCESSFULLY] #" << endl << endl;
}
