#include <iostream>
#include <string>
#include "jlparser/parser.hpp"
#include <ctime>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Kozei.hpp"
#include "Basket.hpp"
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "ParameterEstimation.hpp"
#include <assert.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

using namespace std;
using namespace Computations;

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

  // Get AllData
  ParameterEstimation p;
  str4 = str3+"market-data/market-data/kozei.dat";
  const char *cstr = str4.c_str();
  PnlMat *indexDate = pnl_mat_create_from_file();
  PnlMat *corr = pnl_mat_create_from_file(cstr);


  double T, r, inv_init;
  PnlVect *spot, *sigma, *divid, *payOffCoeff;
  int size;
  size_t n_samples;
  int n_timestep;
  double h = 0.1;

  // undeclared rho
  double rho;

  T = 1;
  n_timestep = 1;
  rho = 0;
  r=0.04879;
  inv_init = 100;
  size=1;
  n_samples=50000;

  spot = pnl_vect_create_from_scalar (1, 100.0);
  sigma = pnl_vect_create_from_scalar (1, 0.20);
  divid = pnl_vect_create_from_scalar (1, 0);
  payOffCoeff = pnl_vect_create_from_scalar (1, 1.0);



  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));

  BlackScholesModel bc = BlackScholesModel(size, r, rho, sigma, spot);
  Kozei kozei_test = Kozei(inv_init);
  MonteCarlo mc_test = MonteCarlo(&bc , &kozei_test, rng, h, n_samples);

  PnlVect *delta = pnl_vect_create_from_scalar(size, 0.0);
  PnlVect *icdelta = pnl_vect_create_from_scalar(size, 0.0);

  double prix_ent = 0.0;
  double prix_0 = 0.0;

  double ic_ent = 0.0;
  double ic_0 = 0.0;

  str3 = str3+"market-data/market-data/simul_call.dat";
  const char *cstr = str3.c_str();

  PnlMat *past = pnl_mat_create_from_file(cstr);

  mc_test.price(past, 200.0*T/365, prix_ent, ic_ent);
  mc_test.delta(past, 200.0*T/365, delta, icdelta);

  PnlVect *V = pnl_vect_create(size);
  pnl_mat_get_row(V, past, past->m -1);
  BlackScholesModel bc2 = BlackScholesModel(size, r, rho, sigma, V);
  Basket basket_test2 = Basket(T-200.0*T/365, n_timestep , size, inv_init, payOffCoeff);
  MonteCarlo mc_test2 = MonteCarlo(&bc2 , &basket_test2,  rng, 0.1,n_samples);

  mc_test2.price(prix_0,ic_0);

  cout << "Delta :" << endl;
  pnl_vect_print_asrow(delta);

  cout<<"Prix calculer par la fct price(0) :"<<prix_0<<endl;
  cout<<"Prix calculer par la fct price(t) :"<<prix_ent<<endl;

  cout<<"IC par la fct price(0) :"<<ic_0<<endl;
  cout<<"IC par la fct price(t) :"<<ic_ent<<endl;

  pnl_vect_free(&V);

}
