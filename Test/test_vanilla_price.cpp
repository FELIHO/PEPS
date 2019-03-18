#include <stdlib.h>
#include "Basket.hpp"
#include "MonteCarlo.hpp"

using namespace std;

int main(){

    double T, r, strike, rho,prix_0,ic_0;
    PnlVect *spot, *sigma,*payOffCoeff;
    int size;
    size_t n_samples;
    int n_timestep;
    double h = 0.1;

    T = 1;
    n_timestep = 1;
    rho = 0;
    strike  = 100;
    r=0.1; 

    sigma = pnl_vect_create_from_scalar (1, 0.20);
    payOffCoeff = pnl_vect_create_from_scalar (1, 1.0);
    size=1;
    n_samples=50000;

    PnlVect* call_prices_pricer =  pnl_vect_create(51);
    PnlVect* call_prices_BS = pnl_vect_create(51);
    PnlVect* prices_diff =  pnl_vect_create(51);

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    for(int i = 0;i<=50;i++){
        spot = pnl_vect_create_from_scalar (1, i*4);
        BlackScholesModel bc = BlackScholesModel(size, r, rho, sigma, spot);
        Basket basket_test = Basket(T, n_timestep , size, strike, payOffCoeff);
        MonteCarlo mc_test = MonteCarlo(&bc , &basket_test,h, n_samples);
        double price_BS_formula =  basket_test.price_formuleBS( &bc, T, strike,r,pnl_vect_get(sigma,0) ) ;
        mc_test.price(prix_0,ic_0);
        pnl_vect_set (call_prices_pricer,i, prix_0);
        pnl_vect_set (call_prices_BS,i, price_BS_formula);
        pnl_vect_set (prices_diff,i, abs(prix_0-price_BS_formula));
        delete(&bc);
        delete(&mc_test);

    }


    cout<<"Prix du call par la formule de B&S  : "<<endl;
    pnl_vect_print(call_prices_pricer);
    cout<<"Prix du call par le pricer  : "<<endl;
    pnl_vect_print(call_prices_BS);
    cout<< " prix_0-price_formuleBS "<<endl;
    pnl_vect_print(prices_diff);

    pnl_vect_free(&call_prices_pricer);
    pnl_vect_free(&call_prices_BS);
    pnl_vect_free(&prices_diff);


}
