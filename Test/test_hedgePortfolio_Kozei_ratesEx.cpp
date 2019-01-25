#include <iostream>
#include <string>
#include <ctime>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "DataSelecter.hpp"
#include "ParameterEstimation.hpp"
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "Kozei.hpp"
#include "HedgePortfolio.hpp"
#include <assert.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

using namespace std;


int main(){

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
    } else {
        perror("getcwd() error");
        return 1;
    }

    string way;
    way.assign(cwd);
    size_t pos = way.find("/PEPS/");
    string str3 = way.substr(0,pos+6);
    string str4 = way.substr(0,pos+6);
    string str5 = way.substr(0,pos+6);

    str3 = str3+"DataFeeds/kozei_dataFeed.dat";
    const char *cstr = str3.c_str();
    PnlMat *allData = pnl_mat_create_from_file(cstr);

    str4 = str4+"DataFeeds/kozei_IndexdataFeed.dat";
    const char *cstr2 = str4.c_str();
    PnlVectInt *dateIndexes = pnl_vect_int_create_from_file(cstr2);

    str5 = str5+"DataFeeds/kozei_Constatation.dat";
    const char *cstr3 = str5.c_str();
    PnlVectInt *constationDate = pnl_vect_int_create_from_file(cstr3);

    DataSelecter ds_Assets = DataSelecter(constationDate);



    int date = 20180515;
    PnlMat *past = ds_Assets.getPast(allData, dateIndexes, date);



    double inv_init = 100;

    int timeEstimation = 365;

    ParameterEstimation pe_Assets;
    PnlMat window_Assets = ds_Assets.getWindowPreviousEstimation(allData, dateIndexes, date, timeEstimation);
    PnlMat* corr_Assets = pe_Assets.getCorrelationMatrix(&window_Assets);
    PnlVect* sigma_Assets = pe_Assets.getVolatilitiesVector(&window_Assets);


    PnlVect* spot = pnl_vect_new();
    pnl_mat_get_row(spot, past, 0);


    double r = 0.01;

    PnlRng *Pnlrng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(Pnlrng, time(NULL));
    RandomGen *rng = new PnlRnd(Pnlrng); 

    size_t n_samples = 50000;
    double h = 0.1;
    // Maturité from 0 is 8 year and 11 day
    double T = 8.0 + 11.0/365;
    double t = ds_Assets.computeTvalue(dateIndexes, pnl_vect_int_get(constationDate, 0), date, T);

    BlackScholesModel *bc_Assets = new BlackScholesModel(past->n, r, corr_Assets, sigma_Assets, spot);

    Kozei *kozei_test = new Kozei(inv_init);
    MonteCarlo *mc_test = new MonteCarlo(bc_Assets , kozei_test, rng,h, n_samples);

    PnlMat *DataSimu = pnl_mat_create(17,30);
    //Taux de change de la date 25/01/2019
    PnlVect* interestRate_USD = pnl_vect_create_from_double(DataSimu->m,0.88);
    PnlVect* interestRate_JPY = pnl_vect_create_from_double(DataSimu->m,0.0080);
    PnlVect* interestRate_GBP = pnl_vect_create_from_double(DataSimu->m,1.15);
    PnlVect* interestRate_CHF = pnl_vect_create_from_double(DataSimu->m,0.88);
    PnlVect* interestRate_BRL = pnl_vect_create_from_double(DataSimu->m,0.23);

    PnlMat *InterestRates = pnl_mat_create(DataSimu->m,5);

    pnl_mat_set_col(InterestRates,interestRate_USD,0);
    pnl_mat_set_col(InterestRates,interestRate_JPY,1);
    pnl_mat_set_col(InterestRates,interestRate_GBP,2);
    pnl_mat_set_col(InterestRates,interestRate_CHF,3);
    pnl_mat_set_col(InterestRates,interestRate_BRL,4);

    bc_Assets->asset(DataSimu, T, 16,rng);

    PnlMat* DomesticAssets = pe_Assets.getDomesticAssetPrices(DataSimu,InterestRates);
    pnl_mat_print(DataSimu);
    cout<<" "<<endl;
    pnl_mat_print(DomesticAssets);
    HedgePortfolio *hedgePortfolio = new HedgePortfolio(DomesticAssets, mc_test);
    double PL = hedgePortfolio->HedgeError(DomesticAssets);

    cout<<"P&L" << PL<<endl;

    pnl_mat_free(&corr_Assets);
    pnl_vect_free(&sigma_Assets);
    pnl_vect_free(&interestRate_USD);
    pnl_vect_free(&interestRate_JPY);
    pnl_vect_free(&interestRate_GBP);
    pnl_vect_free(&interestRate_CHF);
    pnl_vect_free(&interestRate_BRL);
    pnl_mat_free(&InterestRates);

    delete(kozei_test);
    delete(bc_Assets);
    delete(mc_test);
    delete(hedgePortfolio);
    delete(rng);


}
