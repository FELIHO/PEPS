#pragma once

#ifndef PNLRAND_HPP
#define PNLRAND_HPP

#include "RandomGen.hpp"

/// \brief Classe PnlRand
class PnlRand : public RandomGen
{
private:
    PnlRng* rng_;

public:
    PnlRand(PnlRand const &PnlRandACopier);
    PnlRand(PnlRng* rng);
    void mat_normal(PnlMat* MBS, int nbligne, int nbcol);
    virtual ~PnlRand();
    PnlRand* clone();
};

#endif
