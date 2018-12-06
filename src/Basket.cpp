#pragma once


#include <algorithm>

/// \brief Classe Option abstraite
#include "Option.hpp"
#include "Basket.hpp"

Computations::Basket::Basket():Option(){
  strike_ = 0.0;
  PayOff_coefficient = pnl_vect_new();
}

Computations::Basket::Basket(double T, int nbTimeSteps, int size, double strike , PnlVect *payOff_coefficient):Option(T, nbTimeSteps, size){
  strike_ = strike;
  PayOff_coefficient = pnl_vect_copy(payOff_coefficient);
}

Computations::Basket::Basket(const Basket &B) {
  T_ = B.T_ ;
  nbTimeSteps_ = B.nbTimeSteps_;
  size_ = B.size_;
  strike_ = B.strike_;
  PayOff_coefficient = pnl_vect_copy(B.PayOff_coefficient);
}

Computations::Basket& Computations::Basket::operator=(const Basket &B) {
  T_ = B.T_ ;
  nbTimeSteps_ = B.nbTimeSteps_;
  size_ = B.size_;
  strike_ = B.strike_;
  PayOff_coefficient = B.PayOff_coefficient;
  return *this ;
}

Computations::Basket::~Basket() {
  pnl_vect_free(&PayOff_coefficient);
}

double Computations::Basket :: payoff(const PnlMat *path){
    PnlVect lastLine = pnl_vect_wrap_mat_row(path, nbTimeSteps_);
    double ScalarProductResult = pnl_vect_scalar_prod(&lastLine, PayOff_coefficient);
    double result = ScalarProductResult - strike_;
    return std::max(result,0.0);
}
