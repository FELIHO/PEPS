#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Delta_LH.hpp"
using namespace Computations;

Delta_LH::Delta_LH() {

}



Delta_LH::~Delta_LH() {
}


Delta_LH::Delta_LH(const Delta_LH &O) {

}

double Delta_LH::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic, AssetModel *mod, Option *opt, PnlRng *rng, int nbSamples) {
	// TO DO NOT YET IMPLEMENTED
}