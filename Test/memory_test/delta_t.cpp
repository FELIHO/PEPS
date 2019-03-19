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

    int size = 2;
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


    //construction de past: [[5, 5][2, 2]]
    PnlMat* past = pnl_mat_create_from_scalar(2, size, 2);
    pnl_mat_set_row(past, spots, 0);

    PnlVect* delta = pnl_vect_new();

    mc_pricer->delta(past,1,delta);

    //pnl_vect_print(delta);

    pnl_vect_free(&delta);
    pnl_vect_free(&sigma);
    pnl_vect_free(&spots);
    delete(blackScholesModel);
    pnl_vect_free(&weights);
    delete(option_Basket);
    delete(mc_pricer);

}