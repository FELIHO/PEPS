#pragma once

#ifndef RANDOMGEN_HPP
#define RANDOMGEN_HPP

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

#include <iostream>
#include <stdio.h>


/// \brief Classe RandomGen abstraite
class RandomGen
{
public:
    //RandomGen();
    virtual ~RandomGen() {};
    //RandomGen(RandomGen const &RandomGenACopier);
    virtual void mat_normal(PnlMat* MBS, int nbligne, int nbcol) = 0;
    virtual RandomGen* clone() = 0;
};


#endif
