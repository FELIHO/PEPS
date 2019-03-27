#pragma once

#ifndef TEST_HPP
#define TEST_HPP

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "MonteCarlo.hpp"
#include "HedgePortfolio.hpp"
#include "Option.hpp"
#include "DataSelecter.hpp"

/** \class HedgePortfolio
 * \brief Portefeuille de Couverture
 */
class Test
  {
  public:

	MonteCarlo *monteCarlo_;

    DataSelecter* DS_;    

    PnlMat *marketData_;

    virtual ~Test() {};

    virtual void setRebalancementFrequence(int nbRebalancementPerStep);

    virtual double price(double t) = 0;

    virtual PnlVect* delta(double t) = 0;

    virtual double ProfitAndLoss() = 0;

  };

#endif