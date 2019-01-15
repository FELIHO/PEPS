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
    PnlVect *sigma =  pnl_vect_create_from_scalar(size, 1);
    PnlVect *spots = pnl_vect_create_from_scalar(size, 5);

    //simulation d'un modele de Blanck&Scholes
    BlackScholesModel* blackScholesModel = new BlackScholesModel(size, r, rho, sigma, spots);

    PnlMat* shift_path = pnl_mat_create(nbTimeSteps+1, size);
    PnlMat* path = pnl_mat_create_from_scalar(nbTimeSteps+1, size, 1);


    int t = 1; //date à laquelle je shift
    int d = 1; //l'actif à shifter
    int h = 100;
    blackScholesModel->shiftAsset(shift_path, path, d, h, t, nbTimeSteps);

    PnlMat* shift_pathCal = pnl_mat_create_from_scalar(nbTimeSteps+1, size, 1);
    PnlVect* V = pnl_vect_create_from_scalar(nbTimeSteps+1, (h+1));
    for (int j = 0; j < t; j++) {
      pnl_vect_set(V, j, 1);
    }
    pnl_mat_set_col(shift_pathCal, V, d);

    bool b = true;
    for (int i = 0; i < nbTimeSteps+1; i++) {
      for (int j = 0; j < size; j++) {
        if (pnl_mat_get(shift_path,i, j) != pnl_mat_get(shift_pathCal,i, j)) {
          b = false;
        }
      }
    }

    pnl_mat_free(&shift_path);
    pnl_mat_free(&shift_pathCal);
    pnl_vect_free(&V);
    pnl_mat_free(&path);
    delete(blackScholesModel);


    assert(b = true);
    cout << "########################################" << endl;
    cout << "# [test_unit_shift TEST PASSED SUCCESSFULLY] #" << endl;
    cout << "########################################" << endl <<endl;

}
