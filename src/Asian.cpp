#include <algorithm>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Classe Option abstraite
#include "Option.hpp"
#include "Asian.hpp"
using namespace Computations;

Asian::Asian():Option(){
  PayOff_coefficient = pnl_vect_new();
  strike_ = 0.0;
}

Asian::Asian(double T, int nbTimeSteps, int size, double strike, PnlVect *payOff_coefficient):Option(T, nbTimeSteps, size){
  PayOff_coefficient = pnl_vect_copy(payOff_coefficient);
  strike_ = strike;
}

Asian::Asian(const Asian &A) {
  T_ = A.T_ ;
  nbTimeSteps_ = A.nbTimeSteps_;
  size_ = A.size_;
  PayOff_coefficient = pnl_vect_copy(A.PayOff_coefficient);
  strike_ = A.strike_;
}

Asian& Asian::operator=(const Asian &A) {
  T_ = A.T_ ;
  nbTimeSteps_ = A.nbTimeSteps_;
  size_ = A.size_;
  PayOff_coefficient = A.PayOff_coefficient;
  strike_ = A.strike_;
  return *this ;
}

Asian::~Asian() {
  pnl_vect_free(&PayOff_coefficient);
}

double Asian :: payoff(const PnlMat *path){
  PnlVect *vectAverage = pnl_vect_create(size_);
  PnlVect *currentCol = pnl_vect_create(path->m);
  for (int i = 0; i < size_; i++){
    pnl_mat_get_col(currentCol, path, i);
    pnl_vect_set(vectAverage, i, (1.0/currentCol->size)*pnl_vect_sum(currentCol));
  }
  double result = std::max(pnl_vect_scalar_prod(PayOff_coefficient,vectAverage) - strike_,0.0);

  pnl_vect_free(&vectAverage);
  pnl_vect_free(&currentCol);

  return result;
}
