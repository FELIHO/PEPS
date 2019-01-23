#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"


#include "BlackScholesModel.hpp"

using namespace std;
using namespace Computations;

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

    PnlMat *mat1 =  pnl_mat_create_from_scalar(2, size, 1);
    PnlMat *mat2 = pnl_mat_create_from_scalar(2, size, 5);
    PnlMat *res = pnl_mat_create_from_zero(4, size);

    blackScholesModel->concatenationMatrice(res, mat1, mat2);

    PnlMat *resCal = pnl_mat_create_from_zero(4, size);
    for (int i = 0; i < 2; i++) {
      PnlVect* v = pnl_vect_create(size);
      pnl_mat_get_row(v, mat1, i);
      pnl_mat_set_row(resCal, v, i);
    }
    for (int i = 2; i < 4; i++) {
      PnlVect* v = pnl_vect_create(size);
      pnl_mat_get_row(v, mat2, i-2);
      pnl_mat_set_row(resCal, v, i);
    }

    bool b = true;
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < 4; j++) {
        if (pnl_mat_get(res,i, j) != pnl_mat_get(resCal,i, j)) {
          b = false;
        }
      }
    }

    pnl_mat_free(&mat1);
    pnl_mat_free(&mat2);
    pnl_mat_free(&res);
    pnl_mat_free(&resCal);
    delete(blackScholesModel);

    assert(b = true);
    cout << "########################################" << endl;
    cout << "# [test_unit_concatenationMatrice TEST PASSED SUCCESSFULLY] #" << endl;
    cout << "########################################" << endl <<endl;

}
