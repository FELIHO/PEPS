#include "Option/Basket.hpp"
#include "MonteCarlo/MonteCarlo.hpp"

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
  double correct_price = 13.616294;
  //double payoff = 0;
  double prix;
  double ic;

  char const *infile = "../../data/basket_1.dat";
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

  PnlVect* deltas_correct = pnl_vect_create_from_list(size, 0.0248956, 0.0249324, 0.0249503, 0.0250191, 0.0249342, 0.024961, 0.0249808, 0.0248986, 0.0249032, 0.0249421, 0.0249554, 0.0249416, 0.0249236, 0.0249541, 0.0249515, 0.0250078, 0.0248439, 0.0248783, 0.0249421, 0.0249178, 0.0249168, 0.0249511, 0.0249393, 0.0250007, 0.0249344, 0.0248973, 0.0249136, 0.0249047, 0.024927, 0.0248953, 0.0249739, 0.024962, 0.0249467, 0.0249322, 0.0249571, 0.0249858, 0.0248853, 0.024914, 0.024861, 0.0250143);
  PnlVect* ic_delta = pnl_vect_create(size);
/* ######################################################################## */
/* ######################################################################## */

  // TEST BODY
  // #########

  // Initializing Option
  Basket *test_Basket_1 = new Basket(T, nbTimeSteps, size, strike, weights);

  // Initializing Random Number Generator
  PnlRng* pnlRng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(pnlRng, time(NULL));
  RandomGen* rng = new PnlRand(pnlRng);


  BlackScholesModel *bs_model = new BlackScholesModel(size, r, rho, sigma, spot);
  // bs_model->asset(path, T, nbTimeSteps, rng); // Simulating the path from spot t=0 only
  // payoff = test_Basket_1->payoff(path);

  MonteCarlo *mc_pricer = new MonteCarlo(bs_model, test_Basket_1, rng, 1, n_samples);
  mc_pricer->price(prix, ic);

  // Initializing the past with the spot
  PnlMat* past = pnl_mat_create(1, bs_model->size_);
  pnl_mat_set_row(past, bs_model->spot_, 0);

  // delta at t=0
  PnlVect* delta = pnl_vect_new();
  mc_pricer->delta(past, 0, delta);
  // pnl_vect_print(delta);
  // cout << endl;
  // pnl_vect_print(ic_delta);
  // cout << endl;

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

  cout << endl << "### Testing the Pricer for " << "basket_1.dat" << " file input ###" << endl << endl;
  cout << "#######################################################" << endl;

//  cout << "Option Payoff = " << payoff << endl;
  cout << "Price = " << prix << endl;
  cout << "Confidence Interval = " << ic << endl;
  cout<< "Mean of the relative error on the deltas = " << relative_error_mean  << endl;

  cout << "#######################################################" << endl<<endl;

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
  delete(mc_pricer);
  delete P;
  delete(test_Basket_1);

  // TEST PASS VERIFICATION
  // ######################

  assert(correct_price >= prix - ic);
  assert(correct_price <= prix + ic);
  assert(relative_error_bool = true);

  cout << "# [test_basket_1 TEST PASSED SUCCESSFULLY] #" << endl << endl;

}
