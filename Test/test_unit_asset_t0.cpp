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
    int size = 4;
    double r = 0.05;
    double rho = 0.2;
    double T = 10;
    int nbTimeSteps = 12;
    double pas = T/nbTimeSteps;

    PnlVect *sigma =  pnl_vect_create_from_scalar(size, 1);
    PnlVect *spots = pnl_vect_create_from_scalar(size, 5);
    BlackScholesModel* blackScholesModel = new BlackScholesModel(size, r, rho, sigma, spots);

    PnlMat* path = pnl_mat_create(nbTimeSteps+1, size);

    FakeRnd* rng = new FakeRnd(0.3);
    blackScholesModel->asset(path, T, nbTimeSteps, rng);

    PnlVect* B = pnl_vect_new();
    pnl_mat_sum_vect(B, blackScholesModel->chol_,'c');
    pnl_vect_mult_scalar(B,rng->val_);

    PnlMat* pathCal = pnl_mat_create_from_zero(nbTimeSteps+1, size);
    pnl_mat_set_row(pathCal, spots, 0);
    bool b = true;
    for (int i = 0; i < nbTimeSteps+1; i++) {
      for (int j = 0; j < size; j++) {
        if(i != 0){
          double val =  pnl_mat_get(pathCal, i-1, j)*exp((r-pnl_vect_get(sigma,j)*pnl_vect_get(sigma,j)/2)*pas+pnl_vect_get(sigma,j)*sqrt(pas)*pnl_vect_get(B, j));
          pnl_mat_set(pathCal, i, j, val);
        }
        if (pnl_mat_get(path,i, j) != pnl_mat_get(pathCal,i, j)) {
          b = false;
        }
      }
    }
    pnl_vect_free(&B);
    pnl_mat_free(&path);
    pnl_mat_free(&pathCal);
    delete(blackScholesModel);

    assert(b = true);

    cout << "########################################" << endl;
    cout << "# [test_unit_asset_t0 TEST PASSED SUCCESSFULLY] #" << endl;
    cout << "########################################" << endl <<endl;

}
