#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

#include "RandomGen.hpp"
#include "FakeRnd.hpp"
#include "PnlRnd.hpp"
#include "BlackScholesModel.hpp"

using namespace std;

int main(int argc, char **argv)
{
    // int size = 4;
    // double r = 0.05;
    // double rho = 0.2;
    // PnlVect *sigma =  pnl_vect_create_from_scalar(size, 1);
    // PnlVect *spots = pnl_vect_create_from_scalar(size, 5);
    BlackScholesModel* blackScholesModel =new BlackScholesModel(4, 0.05, 0.2, pnl_vect_create_from_scalar(4, 1), pnl_vect_create_from_scalar(4, 5));

    // double T = 10;
    // int nbTimeSteps = 12;

    PnlMat* path = pnl_mat_create(13,4);
    // PnlRng* pnlRng = pnl_rng_create(PNL_RNG_MERSENNE);
    // pnl_rng_sseed(pnlRng, time(NULL));
    // RandomGen* rng = new PnlRnd(pnlRng);
    RandomGen* rng = new FakeRnd(0.2);
    blackScholesModel->asset(path, 1, 12, rng);


    //pnl_mat_print(path);

    pnl_mat_free(&path);
    delete(blackScholesModel);
    delete(rng);
}