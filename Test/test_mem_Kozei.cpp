#include "Kozei.hpp"
#include "BlackScholesModel.hpp"

#include <iostream>
#include <string>
#include <fstream>


using namespace std;


int main() {
    int size = 30;
    double r = 0.1;

    PnlMat *rho = pnl_mat_create(size, size);
    pnl_mat_set_id(rho);
    double sigma = 0.2;

    PnlRng* pnlRng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(pnlRng, time(NULL));



    Kozei *test_kozei = new Kozei(100);

    PnlMat* mat_s0 = pnl_mat_create_from_double(size,19,100);
    PnlVect* niveaux_initaux = pnl_vect_create(test_kozei->size_);
    PnlMat* past = pnl_mat_create(mat_s0->m - 2, mat_s0->n);
    PnlMat* path = pnl_mat_create(17, size);
    for (int i = 0; i < test_kozei->size_; i++) {

        pnl_vect_set(niveaux_initaux, i, 1.0 / 3 * (pnl_mat_get(mat_s0, 0, i) + pnl_mat_get(mat_s0, 1, i) + pnl_mat_get(mat_s0, 2, i)));
    }
    pnl_mat_set_row(past, niveaux_initaux, 0);

    PnlVect *vect_i = pnl_vect_create(mat_s0->n);
    for (int i = 3;i < mat_s0->m;i++) {
        pnl_mat_get_row(vect_i, mat_s0, i);
        pnl_mat_set_row(past, vect_i, i - 2);
    }


    BlackScholesModel *bs_model = new BlackScholesModel(size, r , rho, pnl_vect_create_from_double(size, sigma), niveaux_initaux);
    bs_model->asset(path, 9.0 / 2, test_kozei->T_, 16, pnlRng, past);

    double payoff_kozei = test_kozei->payoff(path);

    pnl_mat_free(&rho);
    pnl_rng_free(&pnlRng);
    delete(test_kozei);
    pnl_mat_free(&mat_s0);
    pnl_vect_free(&niveaux_initaux);
    pnl_mat_free(&past);
    pnl_mat_free(&path);
    pnl_vect_free(&vect_i);
    delete(bs_model);
        
    return 0;
}





