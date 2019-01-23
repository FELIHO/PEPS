#include <iostream>
#include <string>
#include "jlparser/parser.hpp"
#include <ctime>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "DataSelecter.hpp"
#include "Kozei.hpp"
#include "HedgePortfolio.hpp"
#include "HistoricalDataProvider.hpp"
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
  int indexFirstSpot = ds.getIndexSpotDate(dateIndexes);
  int nbDates = 2021;

  double inv_init;
  cout << "Entrez un investissement de départ : " << endl;
  cin >> inv_init;

  int timeEstimation;
  cout << "Entrez une durée fenêtre d'estimation un entier, en nombre de jour: " << endl;
  cin >> timeEstimation;

  double r;
  cout << "choisissez un taux d'intérêt zéro coupon européen (plus tard il faudrait avoir cette donnée sous forme de vecteur dans nos data): " << endl;
  cin >> r;

  Kozei *kozei_test = new Kozei(inv_init);
  HistoricalDataProvider* hdp = new HistoricalDataProvider(kozei_test, allData, r, nbDates, timeEstimation, indexFirstSpot);
  PnlMat* dataFeeds = ds.getPath(hdp->DataFeed_, 17);
  HedgePortfolio *hedgePortfolio = new HedgePortfolio(dataFeeds, hdp->mc_);
  double PL = hedgePortfolio->HedgeError(dataFeeds);

  cout<<"P&L" << PL<<endl;

  pnl_mat_free(&allData);
  pnl_vect_int_free(&constationDate);
  pnl_vect_int_free(&dateIndexes);
  pnl_mat_free(&dataFeeds);
  delete(kozei_test);
  delete(hdp);
  delete(hedgePortfolio);

}
