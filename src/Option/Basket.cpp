/// \brief Classe Option abstraite
#include "Option/Option.hpp"
#include "Option/Basket.hpp"
#include "pch.h"
#include <algorithm>
using namespace Computations;

Basket::Basket() 
{
  weights_ = pnl_vect_new();
  T_ = 0;
  nbTimeSteps_ = 0;
  size_ = 0;
  strike_ = 0;
}

Basket::Basket(const Basket& source) 
{
  weights_ = pnl_vect_copy(source.weights_);
  T_ = source.T_;
  nbTimeSteps_ = source.nbTimeSteps_;
  size_ = source.size_;
  strike_ = source.strike_;
}

Basket& Basket::operator=(const Basket &B) {
  weights_ = B.weights_;
  T_ = B.T_ ;
  nbTimeSteps_ = B.nbTimeSteps_;
  size_ = B.size_;
  strike_ = B.strike_;
  return *this ;
}

Basket::~Basket()
{
  pnl_vect_free(&weights_);
}

Basket::Basket(double T, int nbTimeSteps, int size, double strike, PnlVect *weights)
{
  weights_ = pnl_vect_copy(weights);
  T_ = T;
  nbTimeSteps_ = nbTimeSteps;
  size_ = size;
  strike_ = strike;
}


Basket* Basket::clone()
{
  return (new Basket(*this));
}


double Basket::payoff(const PnlMat *path)
// /!\ Works for the specific case of K<0 (Put basket option)
{
  double res = 0.0;

  // For a Call basket sign=+1, for a Put basket sign=-1
  double sign = 1;
  if (strike_ < 0)
  {
    sign = -1;
  }

  // Creating a vector to contain the final row of the 'path' matrix
  PnlVect *last_date;
  last_date = pnl_vect_create(size_);

  // Charging the last row
  pnl_mat_get_row(last_date, path, nbTimeSteps_);

  res = (sign * pnl_vect_scalar_prod(weights_, last_date)) - strike_;

  // Deleting last_date
  pnl_vect_free(&last_date);

  if (res >= 0)
  {
    return res;
  }
  else
  {
    return 0.0;
  }

}
