#include <stdlib.h>
#include "Basket.hpp"
#include "MonteCarlo.hpp"
#include "ForwardTest.hpp"

using namespace std;


int main(){

    // Option Call
    double T = 1;
    int n_timestep = 1;
    double strike  = 100;
    PnlVect* weights = pnl_vect_create_from_scalar (1, 1.0);
    Basket* call = new Basket(T, n_timestep , 1, strike, weights);

    //blackscholes
    int size =1;
    double r = 0.03;
    double sigmaValue= 0.2;
    double spotValue= 100;
    PnlVect* sigma = pnl_vect_create_from_double(1,sigmaValue);
    PnlVect* spot = pnl_vect_create_from_double(1,spotValue);
    BlackScholesModel* original_bs = new BlackScholesModel(size, r, 0.0 , sigma, spot);
    BlackScholesModel* bs = new BlackScholesModel();
    

    //montecarlo
    double fdStep = 0.1;
    int n_samples = 50000;
    MonteCarlo* mc = new MonteCarlo();

    double price = 0;
    double ic=0;
    double bs_price;

    //Delta
    bs = new BlackScholesModel(*original_bs);
    ofstream delta;
    delta.open ("../Test/validation_test/price_call_formuleBS_output/delta.dat");
    for(int s = 50; s<161; s++){
        bs->spot_ = pnl_vect_create_from_double(1,s);
        mc = new MonteCarlo(bs,call,fdStep,n_samples);
        mc->price(price,ic);
        bs_price = call->price_Call_formuleBS(s,0,r,sigmaValue);
        delta << s << " " << price << " " << bs_price << "\n";
    }
    delta.close();

    //Vega
    bs = new BlackScholesModel(*original_bs);
    ofstream vega;
    vega.open ("../Test/validation_test/price_call_formuleBS_output/vega.dat");
    for(double volatility = 0.01; volatility<1.01; volatility+=0.01){
        bs->sigma_ = pnl_vect_create_from_double(1,volatility);
        mc = new MonteCarlo(bs,call,fdStep,n_samples);
        mc->price(price,ic);
        bs_price = call->price_Call_formuleBS(spotValue,0,r,volatility);
        vega << volatility << " " << price << " " << bs_price << "\n";
    }
    vega.close();

    //rho
    bs = new BlackScholesModel(*original_bs);
    ofstream rho;
    rho.open ("../Test/validation_test/price_call_formuleBS_output/rho.dat");
    for(double interestRate = 0.001; interestRate<0.101; interestRate+=0.001){
        bs->r_ = interestRate;
        mc = new MonteCarlo(bs,call,fdStep,n_samples);
        mc->price(price,ic);
        bs_price = call->price_Call_formuleBS(spotValue,0,interestRate,sigmaValue);
        rho << interestRate << " " << price << " " << bs_price << "\n";
    }
    rho.close();

    //Spread
    bs = new BlackScholesModel(*original_bs);
    ofstream delta;
    delta.open ("../Test/validation_test/price_call_formuleBS_output/spread.dat");
    for(int k = 50; k<161; k++){
        call->strike_ = k;
        mc = new MonteCarlo(bs,call,fdStep,n_samples);
        mc->price(price,ic);
        bs_price = call->price_Call_formuleBS(spotValue,0,r,sigmaValue);
        delta << k << " " << price << " " << bs_price << "\n";
    }
    delta.close();

    // //Theta
    // ForwardTest* FT = new ForwardTest(call, r, 0.0 , sigmaValue, spotValue, n_samples, fdStep);
    // ofstream Theta;
    // Theta.open ("../Test/validation_test/price_call_formuleBS_output/Theta.dat");
    // for(double t = 0; t<call->T_; t+=1.0/Tools::NumberOfDaysPerYear){
    //     price = FT->price(t);
    //     bs_price = call->price_Call_formuleBS(spotValue,t,r,sigmaValue);
    //     Theta << t << " " << price << " " << bs_price << "\n";
    // }
    // Theta.close();
    //delete(FT);


    pnl_vect_free(&weights);
    pnl_vect_free(&sigma);
    pnl_vect_free(&spot);
    delete(original_bs);
    delete(bs);
    delete(mc);
}
