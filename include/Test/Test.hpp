#pragma once

#ifndef TEST_HPP
#define TEST_HPP

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "MonteCarlo.hpp"
#include "HedgePortfolio.hpp"
#include "Option.hpp"
#include "DataSelecter.hpp"

/** \class Test
 * \brief Classe abstraite mère des deux types de tests ForwardTest et BackTest
 */
class Test
  {
  public:

	MonteCarlo *monteCarlo_;

    DataSelecter* DS_;    

    PnlMat *marketData_;

    virtual ~Test() {};

    virtual void setRebalancementFrequence(int nbRebalancementPerStep) = 0;

    virtual double price(double t) = 0;

    virtual PnlVect* delta(double t) = 0;

    virtual double ProfitAndLoss() = 0;

  };

#endif