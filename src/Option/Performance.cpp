#include "pch.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <algorithm>
#include "Performance.hpp"
using namespace Computations;

Performance::Performance() 
{
  weights_ = pnl_vect_new();
  T_ = 0;
  nbTimeSteps_ = 0;
  size_ = 0;
}

Performance::Performance(const Performance& source) 
{
  weights_ = pnl_vect_copy(source.weights_);
  T_ = source.T_;
  nbTimeSteps_ = source.nbTimeSteps_;
  size_ = source.size_;
}

Performance& Performance::operator=(const Performance &P)
{
  weights_ = P.weights_;
  T_ = P.T_;
  nbTimeSteps_ = P.nbTimeSteps_;
  size_ = P.size_;
}

Performance::~Performance()
{
  pnl_vect_free(&weights_);
}

Performance* Performance::clone()
{
  return (new Performance(*this));
}

Performance::Performance(double T, int nbTimeSteps, int size, PnlVect *weights)
{
  weights_ = pnl_vect_copy(weights);
  T_ = T;
  nbTimeSteps_ = nbTimeSteps;
  size_ = size;
}


double Performance::payoff(const PnlMat *path){

  double res = 1.0;
  double perf = 0.0;

  // Creating two vectors to contain two consecutive rows of the 'path' matrix
  PnlVect *spot_date_i;
  PnlVect *spot_date_i_1;

  spot_date_i = pnl_vect_create(size_);
  spot_date_i_1 = pnl_vect_create(size_);

  for (unsigned int i=0 ; i < nbTimeSteps_ ; i++)
  {
    // Charging the rows
    pnl_mat_get_row(spot_date_i_1, path, i);
    pnl_mat_get_row(spot_date_i, path, i+1);
    perf = ( pnl_vect_scalar_prod(weights_, spot_date_i) / pnl_vect_scalar_prod(weights_, spot_date_i_1) ) - 1;
    if (perf >= 0)
    {
    res += perf;
    }
  }

  // Deleting spot_date
  pnl_vect_free(&spot_date_i);
  pnl_vect_free(&spot_date_i_1);

  return res;

}
