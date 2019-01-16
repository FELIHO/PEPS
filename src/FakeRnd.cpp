#include "FakeRnd.hpp"

FakeRnd::FakeRnd(FakeRnd const &FakeRndACopier){
  val_ = FakeRndACopier.val_;
}

FakeRnd::FakeRnd(double d){
  val_= d;
}

FakeRnd::~FakeRnd(){
}

void FakeRnd::mat_normal(PnlMat* MBS, int nbligne, int nbcol)
  {
    pnl_mat_resize(MBS,nbligne,nbcol);
    for (int i = 0; i < nbligne; i++) {
      for (int j = 0; j < nbcol; j++) {
        pnl_mat_set(MBS, i, j, val_);
      }
    }
  }

FakeRnd* FakeRnd::clone()
{
  return new FakeRnd(*this);
}
