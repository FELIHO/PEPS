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

    DataSelecter ds = DataSelecter(constationDate);



    int date;
    cout << "Entrez une date comprise entre 01/01/1998(19980101) et la 07/01/2019(20190107) par défaut si vous prenez 20180515 le test donne une matrice de taille 10(spot + nbDate de constation): " << endl;
    cin >> date;
    // Normally we have 10 size
    PnlMat *past = ds.getPast(allData, dateIndexes, date);
    cout << "voici votre matrice past" << endl;
    cout<< past->m << endl;


    double inv_init;
    cout << "Entrez un investissement de départ : " << endl;
    cin >> inv_init;

    int timeEstimation;
    cout << "Entrez une durée fenêtre d'estimation un entier, en nombre de jour: " << endl;
    cin >> timeEstimation;

    ParameterEstimation pe;
    PnlMat window = ds.getWindowPreviousEstimation(allData, dateIndexes, date, timeEstimation);
    PnlMat* corr = pe.getCorrelationMatrix(&window);
    PnlVect* sigma = pe.getVolatilitiesVector(&window);
    // Le jour spot est le premier jour de la matrice past
    //PnlVect* spot = ds.getSpotFromData(allData, dateIndexes, pnl_vect_int_get(constationDate, 2));
    //PnlVect* spot = ds.getSpotFromData(allData, dateIndexes, date);
    PnlVect* spot = pnl_vect_new();
    pnl_mat_get_row(spot, past, 0);

    double r;
    cout << "choisissez un taux d'intérêt zéro coupon européen (plus tard il faudrait avoir cette donnée sous forme de vecteur dans nos data): " << endl;
    cin >> r;

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    size_t n_samples = 50000;
    double h = 0.1;
    // Maturité from 0 is 8 year and 11 day
    double T = 8.0 + 11.0/365;
    double t = ds.computeTvalue(dateIndexes, pnl_vect_int_get(constationDate, 0), date, T);

    BlackScholesModel *bc = new BlackScholesModel(past->n, r, corr, sigma, spot);
    Kozei *kozei_test = new Kozei(inv_init);
    MonteCarlo *mc_test = new MonteCarlo(bc , kozei_test, rng, h, n_samples);

    PnlMat *DataSimu = pnl_mat_create(17,30);


    bc->asset(DataSimu, T, 16, rng);
    pnl_mat_print(DataSimu);
    HedgePortfolio *hedgePortfolio = new HedgePortfolio(DataSimu, mc_test);
    double PL = hedgePortfolio->HedgeError(DataSimu);

    cout<<"P&L" << PL<<endl;

    pnl_mat_free(&corr);
    pnl_vect_free(&sigma);

    delete(bc);
    delete(mc_test);
    delete(hedgePortfolio);
    delete(rng);


}
