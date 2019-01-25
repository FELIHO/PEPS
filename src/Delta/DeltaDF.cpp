#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pch.h"
#include "DeltaDF.hpp"

double DFTrendtimeSteps;

DeltaDF::DeltaDF(double fdStep) {
	fdStep_ = fdStep;
}



DeltaDF::~DeltaDF() {
}


DeltaDF::DeltaDF(const DeltaDF &O) {
	fdStep_ = O.fdStep_;
}

void DeltaDF::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic_delta, BlackScholesModel *mod, Option *opt, PnlRng *rng, int nbSamples) {
	DFTrendtimeSteps = opt->T_ / opt->nbTimeSteps_;
	pnl_vect_resize(delta, opt->size_);
	pnl_vect_resize(ic_delta, opt->size_);
	PnlMat *path = pnl_mat_create(opt->nbTimeSteps_ + 1, opt->size_);
	PnlMat *shift_path_plus = pnl_mat_new();
	PnlMat *shift_path_minus = pnl_mat_new();

	PnlVect *delta_carre = pnl_vect_create_from_scalar(opt->size_, 0);
	PnlVect *delta_2 = pnl_vect_create_from_scalar(opt->size_, 0);

	double timestep = opt->T_ / opt->nbTimeSteps_;
	double Diff = 0;
	PnlVect *vectDiff = pnl_vect_create_from_scalar(opt->size_, 0);
	PnlVect *vectDiffCarre = pnl_vect_create_from_scalar(opt->size_, 0);

	for (int i = 0; i < nbSamples; i++) {
		mod->asset(path, t, opt->T_, opt->nbTimeSteps_, rng, past);
		for (int d = 0; d < opt->size_; d++) {
			mod->shiftAsset(shift_path_plus, path, d, fdStep_, t, timestep);
			mod->shiftAsset(shift_path_minus, path, d, -fdStep_, t, timestep);
			Diff = opt->payoff(shift_path_plus) - opt->payoff(shift_path_minus);
			pnl_vect_set(vectDiff, d, Diff);

			pnl_vect_set(vectDiffCarre, d, Diff*Diff);
		}
		pnl_vect_plus_vect(delta, vectDiff);

		pnl_vect_plus_vect(delta_carre, vectDiffCarre); // moyenne des carres
	}
	double exp_ = exp(-pnl_vect_get(mod->r_,0)*(opt->T_ - t));
	// calcul delta
	double denominateur = 2 * fdStep_;
	int nbRowsPast = past->m;
	PnlVect* s_t = pnl_vect_new();
	pnl_mat_get_row(s_t, past, nbRowsPast - 1);

	pnl_vect_div_scalar(delta, denominateur);
	pnl_vect_div_scalar(delta, nbSamples);
	pnl_vect_div_vect_term(delta, s_t); // Division par s_t
	//pause delta
	// calcul ic_delta
	pnl_vect_div_scalar(delta_carre, denominateur*denominateur);
	pnl_vect_div_vect_term(delta_carre, s_t);
	pnl_vect_div_vect_term(delta_carre, s_t);
	pnl_vect_div_scalar(delta_carre, nbSamples);

	pnl_vect_plus_vect(delta_2, delta);
	pnl_vect_mult_vect_term(delta_2, delta);

	pnl_vect_plus_vect(ic_delta, delta_carre);
	pnl_vect_minus_vect(ic_delta, delta_2);

	pnl_vect_map_inplace(ic_delta, sqrt);
	pnl_vect_mult_scalar(ic_delta, 2 * 1.96*exp_);

	//fin delta
	pnl_vect_mult_scalar(delta, exp_);


	pnl_vect_free(&s_t);
	pnl_vect_free(&vectDiff);
	pnl_vect_free(&vectDiffCarre);
	pnl_vect_free(&delta_2);
	pnl_vect_free(&delta_carre);
	pnl_mat_free(&path);
	pnl_mat_free(&shift_path_plus);
	pnl_mat_free(&shift_path_minus);
}

double Trenddrift(double TrendElement) {
	return -TrendElement * DFTrendtimeSteps;
}
