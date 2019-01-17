#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pch.h"

/// \brief Classe DeltaPW abstraite
#include "DeltaPW.hpp"
using namespace Computations;

DeltaPW::DeltaPW() {

}



DeltaPW::~DeltaPW() {
}


DeltaPW::DeltaPW(const DeltaPW &O) {

}

void DeltaPW::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic, BlackScholesModel *mod, Option *opt, PnlRng *rng, int nbSamples) {
	// TO DO NOT YET IMPLEMENTED
}
