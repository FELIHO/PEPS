#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "InterestRateModel.hpp"
#include "SimpleRateModel.hpp"
#define DLLEXP   __declspec( dllexport )
using namespace Computations;

SimpleRateModel::SimpleRateModel(): InterestRateModel()
{
}

SimpleRateModel::SimpleRateModel(int size, double r) : InterestRateModel(size, r)
{		
}

SimpleRateModel::SimpleRateModel(int size, PnlVect *r) : InterestRateModel(size, r)
{
}

SimpleRateModel::SimpleRateModel(const SimpleRateModel &SRM) : InterestRateModel(SRM)
{
}

SimpleRateModel::~SimpleRateModel()
{
	pnl_vect_free(&rSpot_);
}


SimpleRateModel& SimpleRateModel::operator = (const SimpleRateModel &SRM) {
	size_ = SRM.size_;
	rSpot_ = SRM.rSpot_;
	return *this;
}

void SimpleRateModel::interest(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng)
{
	pnl_mat_resize(path, nbTimeSteps + 1, size_);
	for (int i = 0; i < nbTimeSteps + 1; i++) {
		pnl_mat_set_row(path, rSpot_, i);
	}
}


void SimpleRateModel::interest(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past)
{
	pnl_mat_resize(path, nbTimeSteps + 1, size_);
	pnl_mat_set_subblock(path, past, 0, 0);
	for (int i = past->m; i < nbTimeSteps + 1; i++) {
		pnl_mat_set_row(path, rSpot_, i);
	}
}
