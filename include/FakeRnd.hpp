#pragma once

#ifndef FAKERND_HPP
#define FAKERND_HPP

#include "RandomGen.hpp"

/// \brief Classe PnlRand
class FakeRnd: public RandomGen
{

  public:
      double val_;
      FakeRnd(double d);
      virtual ~FakeRnd();
      FakeRnd(FakeRnd const &FakeRndACopier);
      void mat_normal(PnlMat* MBS, int nbligne, int nbcol);
      FakeRnd* clone();

  };

#endif
