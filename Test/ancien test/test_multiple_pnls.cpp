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
    MonteCarlo* monteCarlo = new MonteCarlo(BMS, opt_asian, fdStep, nbSamples);

    PnlMat* Dailydata = pnl_mat_new();
    //Simuler le marché pour huit ans,la matrice Data contiendra les prix des actifs journalières pour 8 ans 
    BMS->asset(Dailydata, T, 8*260, rng);

    DataSelecter *DS = new DataSelecter(Dailydata); 

    PnlMat* Data;
    HedgePortfolio *hedgePortfolio;

    //Rebalancement Annuel
    nbRebalancementPerStep = 8;
    Data = DS->getData(T,nbTimeSteps, nbRebalancementPerStep);
    cout << "test Rebalancement Annuelle" << endl;
    cout<<" T : "<<T <<" nbTimeSteps : " << nbTimeSteps<< " nbRebalancementPerStep : "<<nbRebalancementPerStep<<endl;
    PnlVect* PL_values = pnl_vect_create(100);
    for(int i = 0 ; i<100;i++){
        PL=0;
        cout<< " i :"<<i<<endl;
        hedgePortfolio = new HedgePortfolio(Data, monteCarlo);
        PL = hedgePortfolio->HedgeError(Data);
        cout<<PL<<endl;
        pnl_vect_set(PL_values,i,PL);
        delete(hedgePortfolio);
    }
    pnl_vect_print(PL_values);

    
    pnl_mat_free(&Data);
    pnl_mat_free(&Dailydata);
    delete(BMS);
    delete(monteCarlo);

}