#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

#include "RandomGen.hpp"
#include "FakeRnd.cpp"
#include "PnlRand.cpp"

#include "BlackScholesModel.hpp"

using namespace std;

int main(int argc, char **argv)
{

    int size = 2;
    double r = 0.05;
    double rho = 0.2;
    PnlVect *sigma =  pnl_vect_create_from_scalar(size, 1);
    PnlVect *spots = pnl_vect_create_from_scalar(size, 5);
    BlackScholesModel* blackScholesModel = new BlackScholesModel(size, r, rho, sigma, spots);



    double T = 8;
    int nbTimeSteps = 4;


    //Controle du rng
    // FakeRnd* rng = new FakeRnd(0.6);

    PnlRng* pnlRng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(pnlRng, time(NULL));
    RandomGen* rng = new PnlRand(pnlRng);

    //construction de past: [[5, 5][2, 2]]
    PnlMat* past = pnl_mat_create_from_scalar(2, size, 2);
    pnl_mat_set_row(past, spots, 0);

    PnlMat* path = pnl_mat_create(nbTimeSteps+1,size);
    //utilisation de asset pour t>0
    blackScholesModel->asset(path, 1, T, nbTimeSteps, rng, past);


    //pnl_mat_print(path);

    pnl_mat_free(&path);
    pnl_mat_free(&past);
    delete(blackScholesModel);
    pnl_vect_free(&sigma);
    pnl_vect_free(&spots);
    delete(rng);

}