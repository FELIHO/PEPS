#pragma once

#ifndef BACKWARDTEST_HPP
#define BACKWARDTEST_HPP

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "MonteCarlo.hpp"

/** \class HedgePortfolio
 * \brief Portefeuille de Couverture
 */
class BackwardTest
  {
  public:
	 /**
	* Priceur par méthode Monte Carlo utilisé pour la couverture
	*/
	  MonteCarlo *monteCarlo_;
  };

#endif