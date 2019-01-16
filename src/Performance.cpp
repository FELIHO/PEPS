#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <algorithm>
#include "Performance.hpp"
using namespace Computations;

Performance::Performance():Option(){
  PayOff_coefficient = pnl_vect_new ();
}

Performance::Performance(double T, int nbTimeSteps, int size, PnlVect *payOff_coefficient)//:Option(T, nbTimeSteps, size)
{
  PayOff_coefficient = pnl_vect_copy(payOff_coefficient);
}

Performance::Performance(const Performance &P) {
  T_ = P.T_ ;
  nbTimeSteps_ = P.nbTimeSteps_;
  size_ = P.size_;
  PayOff_coefficient = pnl_vect_copy(P.PayOff_coefficient);
}

Performance& Performance::operator=(const Performance &P) {
  T_ = P.T_ ;
  nbTimeSteps_ = P.nbTimeSteps_;
  size_ = P.size_;
  PayOff_coefficient = P.PayOff_coefficient;
  return *this ;
}

Performance::~Performance() {
  pnl_vect_free(&PayOff_coefficient);
}


double Performance::payoff(const PnlMat *path){
  double result = 1.0;
  double currentResult;
  double nominateurResult = 0.0;
  double denominateurResult = 0.0;
  PnlVect currentLine ;
  PnlVect previousLine ;
  for (int i = 1; i <= nbTimeSteps_; i++){
    currentLine = pnl_vect_wrap_mat_row(path, i);
    previousLine = pnl_vect_wrap_mat_row(path, i-1);
    nominateurResult =  pnl_vect_scalar_prod(&currentLine, PayOff_coefficient);
    denominateurResult =  pnl_vect_scalar_prod(&previousLine, PayOff_coefficient);
    currentResult = std::max((nominateurResult/denominateurResult)-1.0,0.0);
    result += currentResult;
  }

  return result ;
}
