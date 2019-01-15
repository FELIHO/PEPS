#include <iostream>
#include <string>
#include "jlparser/parser.hpp"
#include <ctime>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Basket.cpp"
#include "BlackScholesModel.cpp"
#include "MonteCarlo.cpp"
#include <assert.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

using namespace std;

int main(){

  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    } else {
      perror("getcwd() error");
      return 1;
  }

  string way;
  way.assign(cwd);
  size_t pos = way.find("/pricer-skel");
  string str3 = way.substr(0,pos+13);

  double T, r, strike, rho;
  PnlVect *spot, *sigma, *divid, *payOffCoeff;
  string type;
  int size;
  size_t n_samples;
  int n_timestep;
  double h = 0.1;
  str3 = str3+"data/basket.dat";
  const char *cstr = str3.c_str();
  Param *P = new Parser(cstr);

  P->extract("option type", type);
  P->extract("maturity", T);
  P->extract("option size", size);
  P->extract("spot", spot, size);
  P->extract("volatility", sigma, size);
  P->extract("interest rate", r);
  P->extract("correlation", rho);
  if (P->extract("dividend rate", divid, size, true) == false)
  {
      divid = pnl_vect_create_from_zero(size);
  }
  P->extract("strike", strike);
  P->extract("sample number", n_samples);
  P->extract("payoff coefficients", payOffCoeff, size);
  P->extract("timestep number", n_timestep);

  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));

  BlackScholesModel bc = BlackScholesModel(size, r, rho, sigma, spot);
  Basket basket_test = Basket(T, 1000 , size, strike, payOffCoeff);
  MonteCarlo mc_test = MonteCarlo(&bc , &basket_test, n_samples, rng, h);


  char cwd2[PATH_MAX];
  if (getcwd(cwd2, sizeof(cwd2)) != NULL) {
    } else {
      perror("getcwd() error");
      return 1;
  }

  string way2;
  way2.assign(cwd2);
  size_t pos2 = way2.find("/RESSOURCES_CHAMILO/");
  string str32 = way2.substr(0,pos2+20);

  str32 = str32+"market-data/market-data/simul_basket_1.dat";
  const char *cstr2 = str32.c_str();
  PnlMat *past = pnl_mat_create_from_file(cstr2);


  double prix = 0.0;
  double ic = 0.0;

  mc_test.price(past,455*basket_test.T_/basket_test.nbTimeSteps_+0.002,prix,ic);

  cout << prix<<endl;
  pnl_mat_free(&past);



}
