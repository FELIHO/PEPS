#include "pch.h"
#include "PnlRand.hpp"


PnlRand::PnlRand(PnlRng* rng)
{
  rng_ = pnl_rng_copy(rng);
}

PnlRand::PnlRand(PnlRand const &PnlRandACopier){
  rng_ = pnl_rng_copy(PnlRandACopier.rng_);
}

void PnlRand::mat_normal(PnlMat* MBS, int nbligne, int nbcol)
  {
    pnl_mat_rng_normal(MBS, nbligne, nbcol, rng_);
  }

PnlRand::~PnlRand(){
    pnl_rng_free(&rng_);
}

PnlRand* PnlRand::clone()
{
  return (new PnlRand(*this));
}
