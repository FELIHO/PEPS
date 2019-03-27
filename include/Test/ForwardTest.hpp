#pragma once

#ifndef FORWARDTEST_HPP
#define FORWARDTEST_HPP

#include "Test.hpp"
#include "SimulatedDataProvider.hpp"


class ForwardTest : public Test
  {
  public:

    ForwardTest(Option* opt, double r, double rho, double sigmaValue, double spotValue, int n_samples, double fdStep);

    ForwardTest(Option* opt, double r, double rho, PnlVect *sigma, PnlVect *spot, int n_samples, double fdStep);

    ~ForwardTest();

    void setRebalancementFrequence(int nbRebalancementPerStep);

    double price(double t);

    PnlVect* delta(double t);

    double ProfitAndLoss();

  };

#endif