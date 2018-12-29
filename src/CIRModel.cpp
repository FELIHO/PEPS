#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <math.h>
#include <stdexcept>
#include "InterestRateModel.hpp"

#include "CIRModel.hpp"
#define DLLEXP   __declspec( dllexport )
using namespace Computations;
using namespace std;

double CIRFirstTimeSteps;
double CIRtimeSteps;

CIRModel::CIRModel() : InterestRateModel()
{
	speedReversion_ = pnl_vect_new();
	longTermMean_ = pnl_vect_new();
	volatilities_ = pnl_vect_new();
	corr_ = pnl_mat_new();
	chol_ = pnl_mat_new();
}

CIRModel::CIRModel(int size, double r, PnlVect *speedReversion, PnlVect *longTermMean, PnlVect *volatilities, PnlMat *corr) : InterestRateModel(size, r)
{
	speedReversion_ = pnl_vect_copy(speedReversion);
	longTermMean_ = pnl_vect_copy(longTermMean);
	volatilities_ = pnl_vect_copy(volatilities);
	corr_ = pnl_mat_copy(corr);
	initalizeChol();
}

CIRModel::CIRModel(int size, PnlVect *r, PnlVect *speedReversion, PnlVect *longTermMean, PnlVect *volatilities, PnlMat *corr) : InterestRateModel(size, r)
{
	speedReversion_ = pnl_vect_copy(speedReversion);
	longTermMean_ = pnl_vect_copy(longTermMean);
	volatilities_ = pnl_vect_copy(volatilities);
	corr_ = pnl_mat_copy(corr);
	initalizeChol();
}

CIRModel::CIRModel(const CIRModel &VCM) : InterestRateModel(VCM)
{
	speedReversion_ = pnl_vect_copy(VCM.speedReversion_);
	longTermMean_ = pnl_vect_copy(VCM.longTermMean_);
	volatilities_ = pnl_vect_copy(VCM.volatilities_);
	corr_ = pnl_mat_copy(VCM.corr_);
	initalizeChol();
}

CIRModel::~CIRModel()
{
	pnl_vect_free(&rSpot_);
	pnl_vect_free(&speedReversion_);
	pnl_vect_free(&longTermMean_);
	pnl_vect_free(&volatilities_);
	pnl_mat_free(&chol_);
}


CIRModel& CIRModel::operator = (const CIRModel &VCM)
{
	size_ = VCM.size_;
	rSpot_ = VCM.rSpot_;
	speedReversion_ = pnl_vect_copy(VCM.speedReversion_);
	longTermMean_ = pnl_vect_copy(VCM.longTermMean_);
	volatilities_ = pnl_vect_copy(VCM.volatilities_);
	return *this;
}

void CIRModel::initalizeChol() {

	/** Validation de la matrice de corrélation */
	PnlVect *eigenValues = pnl_vect_create(size_);
	PnlMat *eigenVectors = pnl_mat_create(size_, size_);
	bool validatedRho = false;

	pnl_mat_eigen(eigenValues, eigenVectors, corr_, 1);
	for (int i = 0; i < size_; i++) {
		if (GET(eigenValues, i) <= 0) {
			validatedRho = true;
			break;
		}
	}

	pnl_vect_free(&eigenValues);
	pnl_mat_free(&eigenVectors);
	//lever une exeption si  la matrice de corrélation n'est pas diagonalisable
	if (validatedRho) {
		throw length_error("Invalid matrix");
	}

	chol_ = pnl_mat_copy(corr_);
	pnl_mat_chol(chol_);

}

void CIRModel::interest(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng)
{
	// TO DO NOT YET IMPLEMENTED
	pnl_mat_resize(path, nbTimeSteps + 1, size_);
	PnlVect *V = pnl_vect_new();
	PnlVect *U = pnl_vect_new();
	PnlVect *G = pnl_vect_new();
	pnl_mat_set_row(path, rSpot_, 0);
	CIRtimeSteps = T / nbTimeSteps;
	PnlVect *drift_ = pnl_vect_new();
	PnlVect *doubledrift_ = pnl_vect_new();
	pnl_vect_map(drift_, speedReversion_, &CIRdrift); // exp(-speedRversion * (ti - ti-1)
	pnl_vect_map(doubledrift_, drift_, &squared); // exp(-2*speedRversion * (ti - ti-1)
	pnl_vect_map_inplace(doubledrift_, &oneUnaryMinus); //  1 - exp(-2*speedRversion * (ti - ti-1)

	for (int i = 1; i < nbTimeSteps + 1; i++) {
		pnl_vect_rng_normal(G, size_, rng);

		pnl_mat_set_row(path, rSpot_, i);
		pnl_mat_get_row(V, path, i - 1);
		pnl_vect_minus_vect(V, longTermMean_); // rti-1 - V
		pnl_vect_mult_vect_term(V, drift_);  // (rti - 1 - V) * exp(-speedRversion * (ti - ti-1)
		pnl_vect_plus_vect(V, longTermMean_); // V + (rti - 1 - V) * exp(-speedRversion * (ti - ti - 1)

		pnl_vect_map(U, volatilities_, &squared); //  sigma²
		pnl_vect_div_scalar(U, 2.0); // sigma² / 2
		pnl_vect_div_vect_term(U, speedReversion_); // sigma² / 2k
		pnl_vect_mult_vect_term(U, doubledrift_); // (sigma² / 2k) * (1 - exp(-2*speedRversion * (ti - ti-1))
		pnl_vect_map_inplace(U, &unsquared); // sqrt((sigma² / 2k) * (1 - exp(-2*speedRversion * (ti - ti-1)))
		pnl_vect_mult_vect_term(U, pnl_mat_mult_vect(chol_, G)); // (Ld | Gi) * sqrt((sigma² / 2k) * (1 - exp(-2*speedRversion * (ti - ti-1)))

		pnl_vect_plus_vect(V, U); // V + (rti - 1 - V) * exp(-speedRversion * (ti - ti - 1) + epsilon * sqrt((sigma² / 2k) * (1 - exp(-2*speedRversion * (ti - ti-1)))
		pnl_mat_set_row(path, V, i);
	}

	pnl_vect_free(&drift_);
	pnl_vect_free(&doubledrift_);
	pnl_vect_free(&G);
	pnl_vect_free(&U);
	pnl_vect_free(&V);
}


void CIRModel::interest(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past)
{
	// TO DO NOT YET IMPLEMENTED
	pnl_mat_resize(path, nbTimeSteps + 1, size_);
	pnl_mat_set_subblock(path, past, 0, 0);
	CIRFirstTimeSteps = computeFirstTimeSteps(T / nbTimeSteps, t);


	PnlVect *V = pnl_vect_new();
	PnlVect *U = pnl_vect_new();
	PnlVect *G = pnl_vect_new();

	//if (FirstTimeSteps != 0) {
	// Parce qu'on travaille avec des double
	if (CIRFirstTimeSteps > 0.0001) {
		// Compute first drift
		PnlVect *firstdrift_ = pnl_vect_new();
		PnlVect *firstdoubledrift_ = pnl_vect_new();
		pnl_vect_map(firstdrift_, speedReversion_, &CIRfirstdrift); // exp(-speedRversion * (ti - t)
		pnl_vect_map(firstdoubledrift_, firstdrift_, &squared); // exp(-2*speedRversion * (ti - t)
		pnl_vect_map_inplace(firstdoubledrift_, &oneUnaryMinus); //  1 - exp(-2*speedRversion * (ti - t)

		// Compute first path
		pnl_vect_rng_normal(G, size_, rng);
		pnl_mat_get_row(V, past, past->m - 1);
		pnl_vect_minus_vect(V, longTermMean_); // rt - V
		pnl_vect_mult_vect_term(V, firstdrift_);  // (rt - 1 - V) * exp(-speedRversion * (ti+1 - t)
		pnl_vect_plus_vect(V, longTermMean_); // V + (rt - 1 - V) * exp(-speedRversion * (ti+1 - t)

		pnl_vect_map(U, volatilities_, &squared); //  sigma²
		pnl_vect_div_scalar(U, 2.0); // sigma² / 2
		pnl_vect_div_vect_term(U, speedReversion_); // sigma² / 2k
		pnl_vect_mult_vect_term(U, firstdoubledrift_); // (sigma² / 2k) * (1 - exp(-2*speedRversion * (ti - t))
		pnl_vect_map_inplace(U, &unsquared); // sqrt((sigma² / 2k) * (1 - exp(-2*speedRversion * (ti - t)))
		pnl_vect_mult_vect_term(U, pnl_mat_mult_vect(chol_, G)); // (Ld | Gi) * sqrt((sigma² / 2k) * (1 - exp(-2*speedRversion * (ti - t)))

		pnl_vect_plus_vect(V, U); // V + (rti - 1 - V) * exp(-speedRversion * (ti - t) + epsilon * sqrt((sigma² / 2k) * (1 - exp(-2*speedRversion * (ti - t)))

		pnl_mat_set_row(path, rSpot_, past->m - 1);

		pnl_vect_free(&firstdrift_);
		pnl_vect_free(&firstdoubledrift_);
	}


	// Compute drift
	CIRtimeSteps = T / nbTimeSteps;
	PnlVect *drift_ = pnl_vect_new();
	PnlVect *doubledrift_ = pnl_vect_new();
	pnl_vect_map(drift_, speedReversion_, &CIRdrift); // exp(-speedRversion * (ti - ti-1)
	pnl_vect_map(doubledrift_, drift_, &squared); // exp(-2*speedRversion * (ti - ti-1)
	pnl_vect_map_inplace(doubledrift_, &oneUnaryMinus); //  1 - exp(-2*speedRversion * (ti - ti-1)

	for (int i = past->m; i < nbTimeSteps + 1; i++) {
		pnl_vect_rng_normal(G, size_, rng);
		pnl_mat_get_row(V, path, i - 1); // rti-1
		pnl_vect_minus_vect(V, longTermMean_); // rti-1 - V
		pnl_vect_mult_vect_term(V, drift_);  // (rti - 1 - V) * exp(-speedRversion * (ti - ti-1)
		pnl_vect_plus_vect(V, longTermMean_); // V + (rti - 1 - V) * exp(-speedRversion * (ti - ti - 1)

		pnl_vect_map(U, volatilities_, &squared); //  sigma²
		pnl_vect_div_scalar(U, 2.0); // sigma² / 2
		pnl_vect_div_vect_term(U, speedReversion_); // sigma² / 2k
		pnl_vect_mult_vect_term(U, doubledrift_); // (sigma² / 2k) * (1 - exp(-2*speedRversion * (ti - ti-1))
		pnl_vect_map_inplace(U, &unsquared); // sqrt((sigma² / 2k) * (1 - exp(-2*speedRversion * (ti - ti-1)))
		pnl_vect_mult_vect_term(U, pnl_mat_mult_vect(chol_, G)); // (Ld | Gi) * sqrt((sigma² / 2k) * (1 - exp(-2*speedRversion * (ti - ti-1)))

		pnl_vect_plus_vect(V, U); // V + (rti - 1 - V) * exp(-speedRversion * (ti - ti - 1) + epsilon * sqrt((sigma² / 2k) * (1 - exp(-2*speedRversion * (ti - ti-1)))
		pnl_mat_set_row(path, V, i);
	}

	pnl_vect_free(&drift_);
	pnl_vect_free(&doubledrift_);
	pnl_vect_free(&G);
	pnl_vect_free(&U);
	pnl_vect_free(&V);
}

double CIRfirstdrift(double vectorElement) {
	return exp(-vectorElement * CIRFirstTimeSteps);
}


double CIRdrift(double vectorElement) {
	return exp(-vectorElement * CIRtimeSteps);
}
