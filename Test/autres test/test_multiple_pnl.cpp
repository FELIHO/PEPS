#include "Basket.hpp"
#include "Asian.hpp"
#include "MonteCarlo.hpp"
#include "HedgePortfolio.hpp"
#include "DataSelecter.hpp"
#include <stdlib.h>

using namespace std;
int main(){
    int nbRebalancementPerStep ;
    double PL;
    double T = 8;
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
    Asian* opt_asian = new Asian(T,nbTimeSteps,size,strike,weights);
    BlackScholesModel* BMS = new BlackScholesModel(size, r, rho, sigma, spot);
    MonteCarlo* monteCarlo = new MonteCarlo(BMS, call, fdStep, nbSamples);

    PnlMat* Dailydata = pnl_mat_new();
    //Simuler le marché pour huit ans,la matrice Data contiendra les prix des actifs journalières pour 8 ans
    BMS->asset(Dailydata, T, 8*Tools::NumberOfDaysPerYear, rng);

    DataSelecter *DS = new DataSelecter(Dailydata);

    PnlMat* Data;
    HedgePortfolio *hedgePortfolio;

    //Rebalancement Annuel
    nbRebalancementPerStep = 1;
    Data = DS->getData(T,nbTimeSteps, nbRebalancementPerStep);
    cout << "test Rebalancement Annuelle" << endl;
    cout<<" T : "<<T <<"nbTimeSteps : " << nbTimeSteps<< "nbRebalancementPerStep : "<<nbRebalancementPerStep<<endl;
    hedgePortfolio = new HedgePortfolio(Data, monteCarlo);
    PL = hedgePortfolio->HedgeError(Data);
    cout << "P&L = " << PL << endl;

    //Rebalancement mensuel
    nbRebalancementPerStep = 6;
    Data = DS->getData(T,nbTimeSteps, nbRebalancementPerStep);
    cout << "test Rebalancement mensuel" << endl;
    cout<<" T : "<<T <<"nbTimeSteps : " << nbTimeSteps<< "nbRebalancementPerStep : "<<nbRebalancementPerStep<<endl;
    hedgePortfolio = new HedgePortfolio(Data, monteCarlo);
    PL = hedgePortfolio->HedgeError(Data);
    cout << "P&L = " << PL << endl;

    //Rebalancement hebdomadaire
    nbRebalancementPerStep = 26;
    Data = DS->getData(T,nbTimeSteps, nbRebalancementPerStep);
    cout << "test Rebalancement hebdomadaire" << endl;
    cout<<" T : "<<T <<"nbTimeSteps : " << nbTimeSteps<< "nbRebalancementPerStep : "<<nbRebalancementPerStep<<endl;
    hedgePortfolio = new HedgePortfolio(Data, monteCarlo);
    PL = hedgePortfolio->HedgeError(Data);
    cout << "P&L = " << PL << endl;

    //Rebalancement journalière
    nbRebalancementPerStep = Tools::NumberOfDaysPerYear/2;
    Data = DS->getData(T,nbTimeSteps, nbRebalancementPerStep);
    cout << "test Rebalancement journalière" << endl;
    cout<<" T : "<<T <<"nbTimeSteps : " << nbTimeSteps<< "nbRebalancementPerStep : "<<nbRebalancementPerStep<<endl;
    hedgePortfolio = new HedgePortfolio(Data, monteCarlo);
    PL = hedgePortfolio->HedgeError(Data);
    cout << "P&L = " << PL << endl;

    pnl_mat_free(&Data);
    pnl_mat_free(&Dailydata);
    delete(BMS);
    delete(monteCarlo);
    delete(hedgePortfolio);

}
