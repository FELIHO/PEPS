#include <iostream>
#include <string>
#include <ctime>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "DataSelecter.hpp"
#include "ParameterEstimation.hpp"
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "Kozei.hpp"
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
  size_t pos = way.find("/PEPS/");
  string str3 = way.substr(0,pos+6);
  string str4 = way.substr(0,pos+6);
  string str5 = way.substr(0,pos+6);


  str3 = str3+"DataFeeds/kozei_dataFeed.dat";
  const char *cstr = str3.c_str();
  PnlMat *allData = pnl_mat_create_from_file(cstr);

  str4 = str4+"DataFeeds/kozei_IndexdataFeed.dat";
  const char *cstr2 = str4.c_str();
  PnlVectInt *dateIndexes = pnl_vect_int_create_from_file(cstr2);

  str5 = str5+"DataFeeds/kozei_Constatation.dat";
  const char *cstr3 = str5.c_str();
  PnlVectInt *constationDate = pnl_vect_int_create_from_file(cstr3);

  DataSelecter ds = DataSelecter(constationDate);



  int date = 20180515;
  PnlMat *past = ds.getPast(allData, dateIndexes, date);


  double inv_init = 100;

  int timeEstimation = 365;

  ParameterEstimation pe;
  PnlMat window = ds.getWindowPreviousEstimation(allData, dateIndexes, date, timeEstimation);
  PnlMat* corr = pe.getCorrelationMatrix(&window);
  PnlVect* sigma = pe.getVolatilitiesVector(&window);
  // Le jour spot est le premier jour de la matrice past
  //PnlVect* spot = ds.getSpotFromData(allData, dateIndexes, pnl_vect_int_get(constationDate, 2));
  //PnlVect* spot = ds.getSpotFromData(allData, dateIndexes, date);
  PnlVect* spot = pnl_vect_new();
  pnl_mat_get_row(spot, past, 0);

  double r = 0.01;
  double n_samples = 50000;

  PnlRng *Pnlrng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(Pnlrng, time(NULL));
  RandomGen *rng = new PnlRnd(Pnlrng); 

  double h = 0.1;
  // Maturité from 0 is 8 year and 11 day
  double T = 8.0 + 11.0/365;
  double t = ds.computeTvalue(dateIndexes, pnl_vect_int_get(constationDate, 0), date, T);

  BlackScholesModel *bc = new BlackScholesModel(past->n, r, corr, sigma, spot);
  Kozei *kozei_test = new Kozei(inv_init);
  MonteCarlo *mc_test = new MonteCarlo(bc , kozei_test, rng, h, n_samples);

  PnlVect *delta = pnl_vect_create_from_scalar(past->n, 1.0);
  PnlVect *icdelta = pnl_vect_create_from_scalar(past->n, 0.0);

  double prix_ent = 0.0;
  double prix_0 = 0.0;

  double ic_ent = 0.0;
  double ic_0 = 0.0;    
  pnl_mat_print(past);
  mc_test->delta(past, 0.0, delta, icdelta);

  cout << "Delta :" << endl;
  pnl_vect_print_asrow(delta);
  cout << "IC Delta :" << endl;
  pnl_vect_print_asrow(icdelta);

  pnl_mat_free(&allData);
  pnl_vect_int_free(&constationDate);
  pnl_vect_int_free(&dateIndexes);
  pnl_mat_free(&past);
  pnl_vect_free(&icdelta);
  pnl_vect_free(&delta);
  pnl_vect_free(&sigma);
  pnl_mat_free(&corr);
  pnl_vect_free(&spot);

}
