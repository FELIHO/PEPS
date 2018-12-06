#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Classe Option abstraite
#include "Option.hpp"

Computations::Option::Option() {
  T_ =0.0 ;
  nbTimeSteps_ =0;
  size_ = 0;
}

Computations::Option::Option(double T, int nbTimeSteps, int size ) {
  T_ = T ;
  nbTimeSteps_ = nbTimeSteps;
  size_ = size;
}

/*Option::Option(const Option &O) {
  T_ = O.T_ ;
  nbTimeSteps_ =O.nbTimeSteps_;
  size_ = O.size_;
}*/

/*Option::Option& operator=(const Option &O) {
  T_ = O.T_ ;
  nbTimeSteps_ =O.nbTimeSteps_;
  size_ = O.size_;
  return *this ;
}*/


Computations::Option::~Option() {
}
