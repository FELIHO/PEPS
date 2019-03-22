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

  ParameterEstimation pe;
  PnlMat* corr = pe.getCorrelationMatrix(allData);
  pnl_mat_print(corr);

  //PnlVect* sigma = pe.getVolatilitiesVector(&allData);
  //PnlMat* logMatrix = pe.getLogRendementMatrix(&allData);
  //PnlMat* covMatrix = pe.getCovarianceMatrix(&allData);


  pnl_mat_free(&allData);
  pnl_mat_free(&corr);
  //pnl_mat_free(&logMatrix);
  //pnl_mat_free(&covMatrix);
  //pnl_vect_free(&sigma);

}
