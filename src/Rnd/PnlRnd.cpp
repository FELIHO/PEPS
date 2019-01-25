#include "PnlRnd.hpp"


PnlRnd::PnlRnd(PnlRng* rng)
{
  rng_ = pnl_rng_copy(rng);
}

PnlRnd::PnlRnd(PnlRnd const &PnlRndACopier){
  rng_ = pnl_rng_copy(PnlRndACopier.rng_);
}

void PnlRnd::mat_normal(PnlMat* MBS, int nbligne, int nbcol)
  {
    pnl_mat_rng_normal(MBS, nbligne, nbcol, rng_);
  }

PnlRnd::~PnlRnd(){
    pnl_rng_free(&rng_);
}

PnlRnd* PnlRnd::clone()
{
  return (new PnlRnd(*this));
}
