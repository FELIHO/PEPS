#include <iostream>
#include <string>
#include "jlparser/parser.hpp"
#include <ctime>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "DataSelecter.hpp"
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

  int firstIndexDate = ds.getIndexSpotDate(dateIndexes);

  double r = 0.05;

  HistoricalDataProvider* hdp = new HistoricalDataProvider(allData, r, 2021, 180, firstIndexDate);

  pnl_mat_print(hdp->DataFeed_);

  pnl_mat_free(&allData);
  pnl_vect_int_free(&constationDate);
  pnl_vect_int_free(&dateIndexes);

}
