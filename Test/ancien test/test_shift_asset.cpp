#include <iostream>
#include <string>

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

  T = 1;
  n_timestep = 1;
  rho = 0;
  r = 0.0478;

  spot = pnl_vect_create_from_scalar (1, 100.0);
  sigma = pnl_vect_create_from_scalar (1, 0.20);
  divid = pnl_vect_create_from_scalar (1, 0);
  payOffCoeff = pnl_vect_create_from_scalar (1, 1.0);
  type = "basket";
  size=5;
  n_samples=50000;

  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));



  PnlVect *delta = pnl_vect_create_from_scalar(size, 0.0);

  double prix_ent = 0.0;
  double prix_0 = 0.0;

  double ic_ent = 0.0;
  double ic_0 = 0.0;

  str3 = str3+"market-data/market-data/simul_basket.dat";
  const char *cstr = str3.c_str();
  PnlMat *path = pnl_mat_create_from_file(cstr);
  PnlMat *shiftpath = pnl_mat_create_from_zero(path->m,path->n);
  BlackScholesModel bc = BlackScholesModel(size, r, rho, sigma, spot);

  bc.shiftAsset(shiftpath, path, 2, 0.5, 28.0/365, 1.0/365);
  pnl_mat_minus_mat(shiftpath,path);
  pnl_mat_print(shiftpath);


}
