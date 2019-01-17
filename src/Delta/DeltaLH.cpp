#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pch.h"
#include "DeltaLH.hpp"
using namespace Computations;

DeltaLH::DeltaLH() {

}



DeltaLH::~DeltaLH() {
}


DeltaLH::DeltaLH(const DeltaLH &O) {

}

void DeltaLH::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic, BlackScholesModel *mod, Option *opt, PnlRng *rng, int nbSamples) {
	// TO DO NOT YET IMPLEMENTED
}
