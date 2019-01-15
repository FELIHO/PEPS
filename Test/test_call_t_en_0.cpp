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

  T = 1;
  n_timestep = 1;
  rho = 0;

  spot = pnl_vect_create_from_scalar (1, 100.0);
  sigma = pnl_vect_create_from_scalar (1, 0.20);
  divid = pnl_vect_create_from_scalar (1, 0);
  payOffCoeff = pnl_vect_create_from_scalar (1, 1.0);
  type = "basket";
  size=1;
  n_samples=50000;

  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));

  BlackScholesModel bc = BlackScholesModel(size, r, rho, sigma, spot);
  Basket basket_test = Basket(T, n_timestep , size, strike, payOffCoeff);
  MonteCarlo mc_test = MonteCarlo(&bc , &basket_test, n_samples, rng, h);

  PnlVect *delta = pnl_vect_create_from_scalar(size, 0.0);

  double prix_ent = 0.0;
  double prix_0 = 0.0;

  double ic_ent = 0.0;
  double ic_0 = 0.0;

  str3 = str3+"market-data/market-data/simul_call.dat";
  const char *cstr = str3.c_str();

  PnlMat *past = pnl_mat_create_from_file(cstr);

  mc_test.price(past, 0.0, prix_ent, ic_ent);

  PnlVect *V = pnl_vect_create(size);
  pnl_mat_get_row(V, past, past->m -1);
  BlackScholesModel bc2 = BlackScholesModel(size, r, rho, sigma, V);
  Basket basket_test2 = Basket(T-200.0*T/365, n_timestep , size, strike, payOffCoeff);
  MonteCarlo mc_test2 = MonteCarlo(&bc2 , &basket_test2, n_samples, rng, 0.1);

  mc_test2.price(prix_0,ic_0);

//  mc_test.delta(past, 200.0*T/365, delta);
  cout << "Delta :" << endl;
  pnl_vect_print(delta);

  cout<<"Prix calculer par la fct price(0) :"<<prix_0<<endl;
  cout<<"Prix calculer par la fct price(t) :"<<prix_ent<<endl;

  cout<<"IC par la fct price(0) :"<<ic_0<<endl;
  cout<<"IC par la fct price(t) :"<<ic_ent<<endl;

  pnl_vect_free(&V);

}
