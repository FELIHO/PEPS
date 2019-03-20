#pragma once

#ifndef BACKTEST_HPP
#define BACKTEST_HPP

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "MonteCarlo.hpp"
#include "HedgePortfolio.hpp"
#include "Option.hpp"
#include "DataSelecter.hpp"
#include "ParameterEstimation.hpp"

class BackTest
  {
  public:

	  MonteCarlo *monteCarlo_;

    DataSelecter* DS_;    

    PnlMat *marketData_;

    BackTest(Option* opt, double r, char const *fileData, int firstDateIndex, int n_samples, double fdStep);

    void setRebalancementFrequence(int nbRebalancementPerStep);

    double price(double t);

    PnlVect* delta(double t);

    double ProfitAndLoss();

  };

#endif