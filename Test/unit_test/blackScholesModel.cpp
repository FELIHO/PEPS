#include <gtest/gtest.h>
#include "BlackScholesModel.hpp"

using namespace std;

int size = 4;
double r = 0.05;
double rho = 0.2;
double T = 1;
int nbTimeSteps = 12;
double pas = T/nbTimeSteps;

PnlVect *sigma =  pnl_vect_create_from_scalar(size, 1);
PnlVect *spots = pnl_vect_create_from_scalar(size, 5);
BlackScholesModel* blackScholesModel = new BlackScholesModel(size, r, rho, sigma, spots);

FakeRnd* rng = new FakeRnd(0.3);


TEST(BlackScholesModel, shift) {
    PnlMat* path = pnl_mat_create(nbTimeSteps+1, size);
    blackScholesModel->asset(path, T, nbTimeSteps, rng);

    double t = 0.5/nbTimeSteps; //date à partir de laquelle je shift
    int d = 1;               //l'actif à shifter
    int h = 10;

    PnlMat* shift_path = pnl_mat_new();
    blackScholesModel->shiftAsset(shift_path, path, d, h, t, pas);

    // calcul terme par terme
    PnlMat* shift_pathCal = pnl_mat_copy(path);
    for(int i = 1; i< shift_pathCal->m; i++){
        pnl_mat_set(shift_pathCal, i , d, pnl_mat_get(shift_pathCal, i, d)*(1+h));
    }

    //comparaison des deux simulations
    bool b = pnl_mat_isequal( shift_path , shift_pathCal,0);

    ASSERT_TRUE(b);

    pnl_mat_free(&shift_path);
    pnl_mat_free(&shift_pathCal);
    pnl_mat_free(&path);
}


TEST(BlackScholesModel, asset_t0) {
    PnlMat* path = pnl_mat_create(nbTimeSteps+1, size);
    blackScholesModel->asset(path, T, nbTimeSteps, rng);

    // calcul terme par terme
    PnlVect* B = pnl_vect_new();
    pnl_mat_sum_vect(B, blackScholesModel->chol_,'c');
    pnl_vect_mult_scalar(B,rng->val_);
    PnlMat* pathCal = pnl_mat_create_from_zero(nbTimeSteps+1, size);
    pnl_mat_set_row(pathCal, spots, 0);
    for (int i = 1; i < nbTimeSteps+1; i++) {
      for (int j = 0; j < size; j++) {
          double val =  pnl_mat_get(pathCal, i-1, j)*exp((r-pnl_vect_get(sigma,j)*pnl_vect_get(sigma,j)/2)*pas+pnl_vect_get(sigma,j)*sqrt(pas)*pnl_vect_get(B, j));
          pnl_mat_set(pathCal, i, j, val);
      }
    }

    //comparaison des deux simulations
    bool b = pnl_mat_isequal( path , pathCal,0);

    ASSERT_TRUE(b);

    pnl_vect_free(&B);
    pnl_mat_free(&pathCal);
    pnl_mat_free(&path);
}

TEST(BlackScholesModel, asset_ti_discrete) {
    PnlMat* path = pnl_mat_create(nbTimeSteps+1, size);

    //construction de past: [[5, 5, 5, 5][4, 4, 4, 4]] qui correspond à ti=1/12
    PnlMat* past = pnl_mat_create_from_scalar(2, size, 4);
    pnl_mat_set_row(past, spots, 0);

    //si on déclare un mauvais ti
    double ti = 2.0/nbTimeSteps; // on s'attend à ce que past contient 3 lignes qui correspondent aux dates 0, 1/12, 2/12 sauf que past n'en contient que 2
    ASSERT_THROW(blackScholesModel->asset(path, ti, T, nbTimeSteps, rng, past),length_error);

    //on continue avec le bon ti
    ti = 1.0/nbTimeSteps;
    blackScholesModel->asset(path, ti, T, nbTimeSteps, rng, past);

    // calcul terme par terme
    PnlVect* B = pnl_vect_new();
    pnl_mat_sum_vect(B, blackScholesModel->chol_,'c');
    pnl_vect_mult_scalar(B,rng->val_);

    PnlMat* pathCal = pnl_mat_create_from_zero(nbTimeSteps+1, size);
    PnlVect* v = pnl_vect_new();
    for (int i = 0; i < past->m; i++) {
      pnl_mat_get_row(v, past, i);
      pnl_mat_set_row(pathCal, v, i);
    }
    pnl_vect_free(&v);

    for(int i =past->m; i<nbTimeSteps+1; i++){
        for (int j = 0; j < size; j++) {
            double val =  pnl_mat_get(pathCal, i-1, j)*exp((r-pnl_vect_get(sigma,j)*pnl_vect_get(sigma,j)/2)*pas+pnl_vect_get(sigma,j)*sqrt(pas)*pnl_vect_get(B, j));
            pnl_mat_set(pathCal, i, j, val);
        }
    }

    //comparaison des deux simulations
    bool b = pnl_mat_isequal( path , pathCal,0);
    ASSERT_TRUE(b);
    

    pnl_vect_free(&B);
    pnl_mat_free(&pathCal);
    pnl_mat_free(&path);
}

TEST(BlackScholesModel, asset_t_nonDiscrete) {
    PnlMat* path = pnl_mat_create(nbTimeSteps+1, size);

    //construction de past: [[5, 5, 5, 5][4, 4, 4, 4]] qui correspond à ti=0.5/12
    PnlMat* past = pnl_mat_create_from_scalar(2, size, 4);
    pnl_mat_set_row(past, spots, 0);

    //si on déclare un mauvais ti
    double ti = 1.5/nbTimeSteps; // on s'attend à ce que past contient 3 lignes qui correspondent aux dates 0, 1/12, 1.5/12 sauf que past n'en contient que 2
    ASSERT_THROW(blackScholesModel->asset(path, ti, T, nbTimeSteps, rng, past),length_error);

    //on continue avec le bon ti
    ti = 0.5/nbTimeSteps;          // past contient les deux dates 0, 0.5/12
    blackScholesModel->asset(path, ti, T, nbTimeSteps, rng, past);

    // calcul terme par terme
    PnlVect* B = pnl_vect_new();
    pnl_mat_sum_vect(B, blackScholesModel->chol_,'c');
    pnl_vect_mult_scalar(B,rng->val_);

    PnlMat* pathCal = pnl_mat_create_from_zero(nbTimeSteps+1, size);
    PnlVect* v = pnl_vect_new();
    for (int i = 0; i < past->m-1; i++) {
      pnl_mat_get_row(v, past, i);
      pnl_mat_set_row(pathCal, v, i);
    }
    pnl_vect_free(&v);

    for (int j = 0; j < size; j++) {
      double val =  pnl_mat_get(past, past->m-1 , j)*exp((r-pnl_vect_get(sigma,j)*pnl_vect_get(sigma,j)/2)*0.5/12+pnl_vect_get(sigma,j)*sqrt(0.5/12)*pnl_vect_get(B, j));
      pnl_mat_set(pathCal, past->m-1 , j, val);
    }

    for(int i =past->m; i<nbTimeSteps+1; i++){
        for (int j = 0; j < size; j++) {
            double val =  pnl_mat_get(pathCal, i-1, j)*exp((r-pnl_vect_get(sigma,j)*pnl_vect_get(sigma,j)/2)*pas+pnl_vect_get(sigma,j)*sqrt(pas)*pnl_vect_get(B, j));
            pnl_mat_set(pathCal, i, j, val);
        }
    }

    //comparaison des deux simulations
    bool b = pnl_mat_isequal( path , pathCal,0);
    ASSERT_TRUE(b);
    

    pnl_vect_free(&B);
    pnl_mat_free(&pathCal);
    pnl_mat_free(&path);
}




 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}