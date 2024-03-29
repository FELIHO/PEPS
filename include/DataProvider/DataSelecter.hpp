#pragma once

#ifndef DATASELECTER_HPP
#define DATASELECTER_HPP

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <math.h>
#include "Tools.hpp"
#include "Kozei.hpp"


class DataSelecter
{
public:

    int firstDateIndex_;
    PnlMat* dailyData_;

    DataSelecter();
    DataSelecter(const DataSelecter &DS);
    DataSelecter& operator= (const DataSelecter &DS);
    ~DataSelecter();

    DataSelecter(PnlMat* dailyData);
    DataSelecter(PnlMat* dailyData, int firstDateIndex);

    PnlMat* getData(double T, int H);
	PnlMat* getData(double T,int nbTimeSteps, int nbRebalancementPerStep);
    PnlMat* getEstimationWindow(double t, int nbDays);
    PnlMat* getHistoricalEstimationWindow();
    PnlMat* getPast(double t, double T, int nbTimeStep);
};

#endif
