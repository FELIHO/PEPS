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
  size_t pos = way.find("/RESSOURCES_CHAMILO/");
  string str3 = way.substr(0,pos+20);

  double T, r, strike, rho;
  PnlVect *spot, *sigma, *divid, *payOffCoeff;
  string type;
  int size;
  size_t n_samples;
  int n_timestep;
  double h = 0.1;

  T = 2;
  n_timestep = 1;
  rho = 0.3;
  r = 0.04879;
  strike = 100;


  spot = pnl_vect_create_from_scalar (10, 100.0);
  sigma = pnl_vect_create_from_scalar (10, 0.20);
  divid = pnl_vect_create_from_scalar (10, 0);
  payOffCoeff = pnl_vect_create_from_scalar (10, 0.1);
  size=10;
  n_samples=50000;

  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));

  BlackScholesModel bc = BlackScholesModel(size, r, rho, sigma, spot);
  Basket basket_test = Basket(T, n_timestep , size, strike, payOffCoeff);
  MonteCarlo mc_test = MonteCarlo(&bc , &basket_test, n_samples, rng, h);


  str3 = str3+"market-soutenance/market_basket.dat";
  const char *cstr = str3.c_str();

  PnlMat *past = pnl_mat_create_from_file(cstr);
  double PL = 0;
  int H = 150;

  mc_test.Profit_and_loss(past,PL ,H );

  cout << "P&L basket: "<<PL<<endl;



}
