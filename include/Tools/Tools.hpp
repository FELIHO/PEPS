#pragma once

#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "pnl/pnl_matrix.h"
#include <stdexcept>

using namespace std;



struct Tools
{
    static const int NumberOfDaysPerYear = 260;

    static const int NumberOfWeeksPerYear = 52;

    static const int NumberOfDaysPerWeek = 5;

    static void concatenationMatrice(PnlMat* res, const PnlMat *mat1, const PnlMat *mat2);
};


#endif