#pragma once

#ifndef FORWARDTEST_HPP
#define FORWARDTEST_HPP

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "MonteCarlo.hpp"
#include "SimulatedDataProvider.hpp"
#include "HedgePortfolio.hpp"
#include "Option.hpp"
#include "DataSelecter.hpp"

/** \class HedgePortfolio
 * \brief Portefeuille de Couverture
 */
class ForwardTest
  {
  public:

	  MonteCarlo *monteCarlo_;

    DataSelecter* DS_;    

    PnlMat *marketData_;

    ForwardTest(Option* opt, double r, double rho, double sigmaValue, double spotValue, int n_samples, double fdStep);

    ForwardTest(Option* opt, double r, double rho, PnlVect *sigma, PnlVect *spot, int n_samples, double fdStep);

    ~ForwardTest();

    void setRebalancementFrequence(int nbRebalancementPerStep);

    double price(double t);

    PnlVect* delta(double t);

    double ProfitAndLoss();

  };

#endif