#include <iostream>
#include <string>
#include <assert.h>
#include <sstream>
#include <ctime>
#include "jlparser/parser.hpp"
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Basket.cpp"
#include "BlackScholesModel.cpp"
#include "MonteCarlo.cpp"
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

using namespace std;

int main()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      } else {
        perror("getcwd() error");
        return 1;
    }

    string way;
    way.assign(cwd);
    size_t pos = way.find("/pricer-skel");
    string str3 = way.substr(0,pos+13);
    double h = 0.1;
    double T, r, strike, rho;
    PnlVect *spot, *sigma, *divid, *payOffCoeff;
    string type;
    int size;
    size_t n_samples;
    int n_timestep;

    str3 = str3+"data/basket_1.dat";
    const char *cstr = str3.c_str();
    Param *P = new Parser(cstr);

    P->extract("option type", type);
    P->extract("maturity", T);
    P->extract("option size", size);
    P->extract("spot", spot, size);
    P->extract("volatility", sigma, size);
    P->extract("interest rate", r);
    P->extract("correlation", rho);
    if (P->extract("dividend rate", divid, size, true) == false)
    {
        divid = pnl_vect_create_from_zero(size);
    }
    P->extract("strike", strike);
    P->extract("sample number", n_samples);
    P->extract("payoff coefficients", payOffCoeff, size);
    P->extract("timestep number", n_timestep);

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    BlackScholesModel bc = BlackScholesModel(size, r, rho, sigma, spot);
    Basket basket_test = Basket(T, 1 , size, strike, payOffCoeff);
    MonteCarlo mc_test = MonteCarlo(&bc , &basket_test, n_samples, rng, h);


    double prix = 0.0;
    double ic = 0.0;

    mc_test.price(prix,ic);
    PnlVect *delta = pnl_vect_create_from_scalar(size, 0.0);
    PnlVect *icdelta = pnl_vect_create_from_scalar(size, 0.0);

    PnlMat *past = pnl_mat_create(1,size);
    pnl_mat_set_row(past, spot, 0);
    mc_test.delta(past, 0.0, delta, icdelta);

    pnl_vect_free(&spot);
    pnl_vect_free(&sigma);
    pnl_vect_free(&divid);
    pnl_vect_free(&payOffCoeff);
    pnl_rng_free(&rng);
    delete P;

    prix = round( prix * 1000.0 ) / 1000.0;
    double sd = round( ic/(2.0*1.96) * 1000.0 ) / 1000.0;
    double var = round( pow(ic/(2.0*1.96),2)*n_samples * 1000.0 ) / 1000.0;
    double trueSD = sqrt(32.053411/n_samples);

    cout << "True Price : " << 13.616294 << endl;
    cout << "True Variance : " << 32.053411 << endl;
    cout << "True Standard Deviation : " << trueSD << endl;
    cout << "True Delta : " << "0.0248956 0.0249324 0.0249503 0.0250191 0.0249342 0.024961 0.0249808 0.0248986 0.0249032 0.0249421 0.0249554 0.0249416 0.0249236 0.0249541 0.0249515 0.0250078 0.0248439 0.0248783 0.0249421 0.0249178 0.0249168 0.0249511 0.0249393 0.0250007 0.0249344 0.0248973 0.0249136 0.0249047 0.024927 0.0248953 0.0249739 0.024962 0.0249467 0.0249322 0.0249571 0.0249858 0.0248853 0.024914 0.024861 0.0250143" << endl;
    cout << "Our Price : " << prix << endl;
    cout << "Our Variance : " << var << endl;
    cout << "Our Standard Deviation : " << sd << endl;
    cout << "Our Delta : " << endl;
    pnl_vect_print(delta);

    pnl_mat_free(&past);
    pnl_vect_free(&delta);
    // Le test peut donc échouer 5 fois sur 100 car intervalle de confiance à 95%
    assert((prix >= 13.616294 - trueSD)&&(prix <= 13.616294 + trueSD));
}
