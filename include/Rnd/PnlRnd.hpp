#pragma once

#ifndef PNLRND_HPP
#define PNLRND_HPP

#include "RandomGen.hpp"

/// \brief Classe PnlRand
class PnlRnd : public RandomGen
{
private:
    PnlRng* rng_;

public:
    PnlRnd(PnlRnd const &PnlRndACopier);
    PnlRnd(PnlRng* rng);
    void mat_normal(PnlMat* MBS, int nbligne, int nbcol);
    virtual ~PnlRnd();
    PnlRnd* clone();
};

#endif
