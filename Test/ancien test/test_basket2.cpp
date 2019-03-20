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

    double T, r, strike, rho;
    PnlVect *spot, *sigma, *divid, *payOffCoeff;
    string type;
    int size;
    size_t n_samples;
    int n_timestep;
    double h = 0.1;

    str3 = str3+"data/basket_2.dat";
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
    Basket basket_test = Basket(T, n_timestep , size, strike, payOffCoeff);
    MonteCarlo mc_test = MonteCarlo(&bc , &basket_test, n_samples, rng, h);


    double prix = 0.0;
    double ic = 0.0;

    mc_test.price(prix,ic);

    pnl_vect_free(&spot);
    pnl_vect_free(&sigma);
    pnl_vect_free(&divid);
    pnl_vect_free(&payOffCoeff);
    pnl_rng_free(&rng);
    delete P;

    prix = round( prix * 1000.0 ) / 1000.0;
    double var = round( pow(ic/(2.0*1.96),2)*n_samples * 1000.0 ) / 1000.0;
    double sd = round( ic/(2.0*1.96) * 1000.0 ) / 1000.0;
    double trueSD = sqrt(152.094852/n_samples);

    cout << "True Price : " << 9.238710 << endl;
    cout << "True Variance : " << 152.094852 << endl;
    cout << "True Standard Deviation : " << trueSD << endl;
    cout << "Our Price : " << prix << endl;
    cout << "Our Variance : " << var << endl;
    cout << "Our Standard Deviation : " << sd << endl;

    // Le test peut donc échouer 5 fois sur 100 car intervalle de confiance à 95%
    assert((prix >= 9.238710 - trueSD)&&(prix <= 9.238710 + trueSD));
}
