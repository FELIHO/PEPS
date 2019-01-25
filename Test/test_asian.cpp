#include "Asian.hpp"
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
  double correct_price = 4.67;
  //double payoff = 0;
  double prix;
  double ic;

  char const *infile = "../../data/asian.dat";
  Param *P = new Parser(infile);

  P->extract("option type", type);
  P->extract("maturity", T);
  P->extract("option size", size);
  P->extract("timestep number", nbTimeSteps);

  //PnlMat *path = pnl_mat_create(nbTimeSteps + 1, size);
  PnlVect *weights = pnl_vect_create(size);
  PnlVect *ic_delta = pnl_vect_create(size);

  P->extract("spot", spot, size);
  P->extract("volatility", sigma, size);
  P->extract("interest rate", r);
  P->extract("strike", strike);
  P->extract("sample number", n_samples);
  P->extract("correlation", rho);
  P->extract("payoff coefficients", weights, size);
  double sd = 0.029;
  PnlVect* sd_deltas = pnl_vect_create_from_list(size, 0.001058, 0.001058);
  PnlVect* deltas_correct = pnl_vect_create_from_list(size, 0.281640, 0.281951);

/* ######################################################################## */
/* ######################################################################## */

  // TEST BODY
  // #########

  // Initializing Option
  Asian *test_Asian = new Asian(T, nbTimeSteps, size, strike, weights);

  // Initializing Random Number Generator
  PnlRng* pnlRng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(pnlRng, time(NULL));
  RandomGen* rng = new PnlRnd(pnlRng);

  BlackScholesModel *bs_model = new BlackScholesModel(size, r, rho, sigma, spot);
  // bs_model->asset(path, T, nbTimeSteps, rng); // Simulating the path from spot t=0 only
  // payoff = test_Asian->payoff(path);

  MonteCarlo *mc_pricer = new MonteCarlo(bs_model, test_Asian, rng, 1, n_samples);
  mc_pricer->price(prix,ic);

  // Initializing the past with the spot
  PnlMat* past = pnl_mat_create(1, bs_model->size_);
  pnl_mat_set_row(past, bs_model->spot_, 0);

  // delta at t=0
  PnlVect* delta = pnl_vect_new();
  mc_pricer->delta(past, 0, delta);
  // cout << endl;
  // pnl_vect_print(ic_delta);
  // for (int i = 0; i < size; i++) {
  //   double val = pnl_vect_get(ic_delta, i)/2;
  //   cout << val;
  //   cout << endl;
  //}
  
  cout << endl;
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

  cout << endl << "### Testing the Pricer for " << "asian.dat" << " file input ###" << endl << endl;
  cout << "#######################################################" << endl;

  // cout << "Option Payoff = " << payoff << endl;
  cout << "Price = " << prix << endl;
  cout << "Confidence Interval = " << ic/2 << endl;
  cout<< "Mean of the relative error on the deltas = " << relative_error_mean  << endl;

  cout << "#######################################################" << endl<<endl;

/* ######################################################################## */
/* ######################################################################## */

  // TEST PASS VERIFICATION
  // ######################

  assert(correct_price >= prix - ic);
  assert(correct_price <= prix + ic);
  assert(prix >= correct_price - sd*2);
  assert(prix <= correct_price + sd*2);
  assert(prix <= correct_price + sd*2);
  assert(relative_error_bool = true);
  cout << endl;
  // assert(pnl_vect_get(delta, 0) >= pnl_vect_get(deltas_correct, 0) - pnl_vect_get(sd_deltas, 0)*2);
  // assert(pnl_vect_get(delta, 0) <= pnl_vect_get(deltas_correct, 0) + pnl_vect_get(sd_deltas, 0)*2);
  // assert(pnl_vect_get(delta, 1) >= pnl_vect_get(deltas_correct, 1) - pnl_vect_get(sd_deltas, 1)*2);
  // assert(pnl_vect_get(delta, 1) <= pnl_vect_get(deltas_correct, 1) + pnl_vect_get(sd_deltas, 1)*2);

  // FREEING DYNAMICALLY ALLOCATED VARIABLES
  // #######################################

  // pnl_mat_free(&path);
  pnl_vect_free(&sd_deltas);
  pnl_vect_free(&deltas_correct);
  pnl_mat_free(&past);
  pnl_vect_free(&delta);
  pnl_vect_free(&relative_error_vect);

  delete(bs_model);
  delete(mc_pricer);
  delete P;
  delete(test_Asian);

  cout << "# [test_basket_1 TEST PASSED SUCCESSFULLY] #" << endl << endl;

}
