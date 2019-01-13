#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Classe Delta_PW abstraite
#include "Delta_PW.hpp"
using namespace Computations;

Delta_PW::Delta_PW() {

}



Delta_PW::~Delta_PW() {
}


Delta_PW::Delta_PW(const Delta_PW &O) {

}

double Delta_PW::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic, AssetModel *mod, Option *opt, PnlRng *rng, int nbSamples) {
	// TO DO NOT YET IMPLEMENTED
}