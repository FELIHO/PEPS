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
    double T = 8;
    int nbTimeSteps = 4;
    double pas = T/nbTimeSteps;
    PnlVect *sigma =  pnl_vect_create_from_scalar(size, 1);
    PnlVect *spots = pnl_vect_create_from_scalar(size, 5);

    //simulation d'un modele de Blanck&Scholes
    BlackScholesModel* blackScholesModel = new BlackScholesModel(size, r, rho, sigma, spots);

    //Controle du rng
    FakeRnd* rng = new FakeRnd(0.3);

    //construction de past: [[5, 5][2, 2]]
    PnlMat* past = pnl_mat_create_from_scalar(2, size, 2);
    pnl_mat_set_row(past, spots, 0);

    PnlMat* path = pnl_mat_create(nbTimeSteps+1, size);

    //utilisation de asset pour ti>0
    double ti = 2;
    blackScholesModel->asset(path, ti, T, nbTimeSteps, rng, past);

    //Calcule "à la main"
    PnlVect* B = pnl_vect_new();
    pnl_mat_sum_vect(B, blackScholesModel->choleskyFactor,'c');
    pnl_vect_mult_scalar(B,rng->val_);

    bool b = true;
    PnlMat* pathCal = pnl_mat_create_from_zero(nbTimeSteps+1, size);
    for (int i = 0; i < ti; i++) {
      PnlVect* v = pnl_vect_create(size);
      pnl_mat_get_row(v, past, i);
      pnl_mat_set_row(pathCal, v, i);
      for (int j = 0; j < size; j++) {
        if (pnl_mat_get(path,i, j) != pnl_mat_get(pathCal,i, j)) {
          b = false;
        }
      }
    }




    pnl_mat_free(&past);
    pnl_mat_free(&path);
    pnl_mat_free(&pathCal);
    pnl_vect_free(&B);
    delete(blackScholesModel);

    assert(b = true);
    cout << "########################################" << endl;
    cout << "# [test_unit_asset_ti TEST PASSED SUCCESSFULLY] #" << endl;
    cout << "########################################" << endl <<endl;


}
