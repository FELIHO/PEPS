#pragma once

#ifndef FORWARDTEST_HPP
#define FORWARDTEST_HPP

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "MonteCarlo.hpp"
#include "SimulatedDataProvider.hpp"
#include "HedgePortfolio.hpp"
#include "Option.hpp"
#include "Kozei.hpp"

/** \class HedgePortfolio
 * \brief Portefeuille de Couverture
 */
class ForwardTest
  {
  public:

	MonteCarlo *monteCarlo_;

    SimulatedDataProvider* Simulator_;

    PnlMat* marketData_;

    ForwardTest(Option* opt, RandomGen* rng, PnlVect* spot, double r, double rho, double sigmaValue, double spotValue, int n_samples, double fdStep);

    void simutaleMarket(int nbRebalancementPerStep);

    void simutaleDailyMarket();

    void simutaleWeeklyMarket();

    void simutaleMonthlyMarket();


  };

#endif