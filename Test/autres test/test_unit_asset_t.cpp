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
    int size = 2;
    double r = 0.05;
    double rho = 0.2;
    double T = 4;
    int nbTimeSteps = 2;
    double pas = T/nbTimeSteps;
    PnlVect *sigma =  pnl_vect_create_from_scalar(size, 1);
    PnlVect *spots = pnl_vect_create_from_scalar(size, 5);

    //simulation d'un modele de Blanck&Scholes
    BlackScholesModel* blackScholesModel = new BlackScholesModel(size, r, rho, sigma, spots);

    //Controle du rng
    FakeRnd* rng = new FakeRnd(0.3);

    //construction de past: [[5, 5][2, 2]]
    double duree_past = 2;
    PnlMat* past = pnl_mat_create_from_scalar(duree_past, size, 2);
    pnl_mat_set_row(past, spots, 0);

    PnlMat* path = pnl_mat_create(nbTimeSteps+1, size);
    //utilisation de asset pour t>0
    int time = 1;
    blackScholesModel->asset(path, time, T, nbTimeSteps, rng, past);


    //Calcule "à la main"
    PnlVect* B = pnl_vect_new();
    pnl_mat_sum_vect(B, blackScholesModel->chol_,'c');
    pnl_vect_mult_scalar(B,rng->val_);

    PnlMat* pathCal = pnl_mat_create_from_zero(nbTimeSteps+1, size);
    for (int i = 0; i < duree_past; i++) {
      PnlVect* v = pnl_vect_create(size);
      pnl_mat_get_row(v, past, i);
      pnl_mat_set_row(pathCal, v, i);
    }
    for (int j = 0; j < size; j++) {
      double val =  pnl_mat_get(pathCal, time, j)*exp((r-pnl_vect_get(sigma,j)*pnl_vect_get(sigma,j)/2)*1+pnl_vect_get(sigma,j)*sqrt(1)*pnl_vect_get(B, j));
      pnl_mat_set(pathCal, time, j, val);
    }
    for (int i = time+1; i < nbTimeSteps+1; i++) {
      for (int j = 0; j < size; j++) {
        double val =  pnl_mat_get(pathCal, i-1, j)*exp((r-pnl_vect_get(sigma,j)*pnl_vect_get(sigma,j)/2)*pas+pnl_vect_get(sigma,j)*sqrt(pas)*pnl_vect_get(B, j));
        pnl_mat_set(pathCal, i, j, val);
      }
    }

    bool b = true;
    for (int i = 0; i < nbTimeSteps+1; i++) {
      for (int j = 0; j < size; j++) {
        if (pnl_isequal_rel(pnl_mat_get(path,i, j), pnl_mat_get(pathCal,i, j), 0x0001)==0) {
          b = false;
        }
      }
    }

    pnl_vect_free(&B);
    pnl_mat_free(&path);
    pnl_mat_free(&pathCal);
    delete(blackScholesModel);

    assert(b == true);
    cout << "########################################" << endl;
    cout << "# [test_unit_asset_t TEST PASSED SUCCESSFULLY] #" << endl;
    cout << "########################################" << endl <<endl;


}
