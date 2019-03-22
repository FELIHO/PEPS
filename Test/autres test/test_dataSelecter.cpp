#include <iostream>
#include <string>
#include <ctime>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "DataSelecter.hpp"
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
  PnlVectInt *dataIndexes = pnl_vect_int_create_from_file(cstr2);

  str5 = str5+"DataFeeds/kozei_Constatation.dat";
  const char *cstr3 = str5.c_str();
  PnlVectInt *constationDate = pnl_vect_int_create_from_file(cstr3);

  DataSelecter ds = DataSelecter(constationDate);

  // Normally we have 10 size
  PnlMat *past = ds.getPast(allData, dataIndexes, 20180515);

  pnl_mat_print(past);
  cout<< past->m << endl;



  pnl_mat_free(&allData);
  pnl_vect_int_free(&constationDate);
  pnl_vect_int_free(&dataIndexes);
  pnl_mat_free(&past);

}
