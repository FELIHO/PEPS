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
#include "Performance.cpp"
#include "Asian.cpp"
#include <assert.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>


using namespace std;

int main(){
  // Récupération des paramètres
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

  double T, r, rho, strike;
  PnlVect *spot, *sigma, *divid, *payOffCoeff;
  string type;
  int size;
  size_t n_samples;
  int n_timestep;
  double h = 0.1;

  str3 = str3+"data/basket_1.dat";
  const char *cstr = str3.c_str();
  Param *P = new Parser(cstr);

  P->extract("option type", type);
  P->extract("maturity", T);
  P->extract("option size", size);
  P->extract("spot", spot, size);
  P->extract("volatility", sigma, size);
  P->extract("interest rate", r);
  P->extract("correlation", rho);
  P->extract("strike", strike);
  if (P->extract("dividend rate", divid, size, true) == false)
  {
      divid = pnl_vect_create_from_zero(size);
  }
  P->extract("sample number", n_samples);
  P->extract("payoff coefficients", payOffCoeff, size);
  P->extract("timestep number", n_timestep);

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
  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));


  // Calcul du prix de l'option à un instant t
  BlackScholesModel bc = BlackScholesModel(size, r, rho, sigma, spot);
  Basket basket_test = Basket(T, n_timestep , size, strike, payOffCoeff);
  MonteCarlo mc_test = MonteCarlo(&bc , &basket_test, n_samples, rng, h);

  double prix = 0.0;
  double ic = 0.0;

  double prix0 = 0.0;
  double ic0 = 0.0;

  mc_test.price(prix0, ic0);
  mc_test.price(past, 0.0, prix, ic);

  pnl_vect_free(&spot);
  pnl_vect_free(&sigma);
  pnl_vect_free(&divid);
  pnl_vect_free(&payOffCoeff);
  pnl_mat_free(&past);
  pnl_rng_free(&rng);

  delete P;

  cout << "True Price : " << 13.627 << endl;
  cout << "Price 0 : " << prix0 << endl;
  cout << "Price t = 0 : " << prix << endl;

}
