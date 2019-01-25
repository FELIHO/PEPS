#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

#include "RandomGen.hpp"
#include "FakeRnd.hpp"
#include "PnlRnd.hpp"

#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "Basket.hpp"

using namespace std;

int main(int argc, char **argv)
{
    int size = 4;
    double r = 0.05;
    double rho = 0.2;
    double T = 8;
    int nbTimeSteps = 4;
    PnlVect *sigma =  pnl_vect_create_from_scalar(size, 1);
    PnlVect *spots = pnl_vect_create_from_scalar(size, 5);

    //simulation d'un modele de Blanck&Scholes
    BlackScholesModel* blackScholesModel = new BlackScholesModel(size, r, rho, sigma, spots);

    double strike = 1.5;
    PnlVect *weights =  pnl_vect_create_from_scalar(size, 0.25);
    Basket *option_Basket = new Basket(T, nbTimeSteps, size, strike, weights);


    
    int n_samples = 50000;
    MonteCarlo *mc_pricer = new MonteCarlo(blackScholesModel, option_Basket, 1, n_samples);

    double price;
    double ic;

    mc_pricer->price(price,ic);

    cout << price << endl;
    cout << ic << endl;

    pnl_vect_free(&sigma);
    pnl_vect_free(&spots);
    delete(blackScholesModel);
    pnl_vect_free(&weights);
    delete(option_Basket);
    delete(mc_pricer);

}