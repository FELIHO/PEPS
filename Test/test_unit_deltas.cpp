#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

#include "RandomGen.hpp"
#include "FakeRnd.cpp"
#include "PnlRand.cpp"

#include "MonteCarlo/MonteCarlo.hpp"
#include "Option/Basket.hpp"

using namespace std;

int main(int argc, char **argv)
{
    int size = 2;
    double r = 0.05;
    double rho = 0.2;
    double T = 8;
    int nbTimeSteps = 8;
    double pas = T/nbTimeSteps;
    PnlVect *sigma =  pnl_vect_create_from_scalar(size, 1);
    PnlVect *spots = pnl_vect_create_from_scalar(size, 5);


    BlackScholesModel* blackScholesModel = new BlackScholesModel(size, r, rho, sigma, spots);
    FakeRnd* rng = new FakeRnd(0.3);
    PnlVect *weights = pnl_vect_create_from_scalar(size, 0.5);
    Basket* opt = new Basket(T, nbTimeSteps, size, 0.5, weights);
    MonteCarlo* monteCarlo = new MonteCarlo(blackScholesModel, opt, rng, 0.001, 100);

    PnlMat* past = pnl_mat_create_from_scalar(1, size, 0);
    pnl_mat_set_row(past, spots, 0);
    pnl_mat_print(past);
    cout << endl;
    double prix = 10;
    double ic = 1;
    PnlVect* delta = pnl_vect_new();
    monteCarlo->delta(past, 0, delta);
    monteCarlo->price(past, 0, prix, ic);
    cout << "prix" << endl;
    cout << prix << endl;
    pnl_vect_print(delta);
    cout << endl;

    // pnl_mat_free(&past);
    // pnl_vect_free(&delta);
}
