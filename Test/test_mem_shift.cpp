#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

//#include "RandomGen.hpp"
//#include "FakeRnd.cpp"
//#include "PnlRand.cpp"

#include "BlackScholesModel.hpp"

using namespace std;
using namespace Computations;

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

    PnlMat* shift_path = pnl_mat_new();
    PnlMat* path = pnl_mat_create_from_scalar(nbTimeSteps+1, size, 1);

    int t = 1; //date à laquelle je shift
    int d = 1; //l'actif à shifter
    int h = 100;
    blackScholesModel->shiftAsset(shift_path, path, d, h, t, nbTimeSteps);

    // cout << "path" << endl;
    // pnl_mat_print(path);
    // cout << "shift_path" << endl;
    // pnl_mat_print(shift_path);

    pnl_mat_free(&path);
    pnl_mat_free(&shift_path);
    pnl_vect_free(&sigma);
    pnl_vect_free(&spots);
    delete(blackScholesModel);

}
