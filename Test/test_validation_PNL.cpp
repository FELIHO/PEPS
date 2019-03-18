#include "Basket.hpp"
#include "MonteCarlo.hpp"
#include "HedgePortfolio.hpp"
#include <stdlib.h>

using namespace std;
int main(){
    double T = 1;
    int nbTimeSteps = 1;
    int size = 1;
    double r = 0.04879;
    double rho = 0;
    PnlVect *sigma = pnl_vect_create_from_double(size, 0.2);
    PnlVect *spot = pnl_vect_create_from_double(size, 3);
    PnlVect *weights = pnl_vect_create_from_double(size, 1.0/size);

    double fdStep = 1;
    int nbSamples = 50000;
    double strike = 10;

    PnlRng* pnlRng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(pnlRng, time(NULL));
    RandomGen* rng = new PnlRnd(pnlRng);

    Basket* call = new Basket(T, nbTimeSteps, size, strike, weights);
    BlackScholesModel* BMS = new BlackScholesModel(size, r, rho, sigma, spot);
    MonteCarlo* monteCarlo = new MonteCarlo(BMS, call, fdStep, nbSamples);

    PnlMat* Data = pnl_mat_new();
    BMS->simul_market(Data, T, 365, rng);


    cout << "test" << endl;
    HedgePortfolio *hedgePortfolio = new HedgePortfolio(Data, monteCarlo);
    double PL = hedgePortfolio->HedgeError(Data);

    cout << "P&L = " << PL << endl;

    delete(BMS);
    delete(monteCarlo);
    delete(hedgePortfolio);

}