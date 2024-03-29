#include "BlackScholesModel.hpp"
#include <gtest/gtest.h>
#include "ParameterEstimation.hpp"


using namespace std;
    int nb_timestep;
    double r ,T,rho;
    PnlVect *sigma_estimated;
    PnlMat *correlation_estimated;
    int size = 30; 

TEST(Estimation, Covariance) {
    double rho = 0;
    T = 8;
    nb_timestep = T * Tools::NumberOfDaysPerYear;
    PnlVect* difference_abs = pnl_vect_create_from_double(size,0.0);
    PnlVect* moy_difference_abs = pnl_vect_create_from_double(60,0.0); 
    PnlVect* sigma = pnl_vect_create_from_scalar(size,0.2);
    PnlVect* spot = pnl_vect_create_from_scalar(size,100.);
    r=0.1; 
    
    PnlRng *Pnlrng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(Pnlrng, time(NULL));
    RandomGen* rng = new PnlRnd(Pnlrng);

    ParameterEstimation *PrEs = new ParameterEstimation();

    BlackScholesModel *bc = new  BlackScholesModel(size, r ,  rho, sigma, spot);

    PnlMat *path = pnl_mat_create(nb_timestep + 1,size);
        
    // 260 is the number of working days per year
    bc->asset(path, T, nb_timestep, rng);
    sigma_estimated =  PrEs->getVolatilitiesVector(path);
    for(int i = 0;i <size;i++){
        //assertion
        EXPECT_NEAR(pnl_vect_get(sigma_estimated,i),pnl_vect_get(sigma,i),pnl_vect_get(sigma,i)*0.1); 
    }
        
    
    pnl_vect_free(&sigma_estimated);
    pnl_vect_free(&sigma);
    pnl_vect_free(&difference_abs);
    pnl_mat_free(&path);
}

TEST(Estimation, Correlation) {
    double rho = 0.8;
    PnlMat* correlation_mat = pnl_mat_create_from_scalar (size, size, rho);
    for (int i =0;i<size;i++){
        pnl_mat_set(correlation_mat,i,i,1);
    }
    T = 8;
    nb_timestep = T * Tools::NumberOfDaysPerYear;
    PnlVect* difference_abs = pnl_vect_create_from_double(size,0.0);
    PnlVect* moy_difference_abs = pnl_vect_create_from_double(60,0.0); 
    PnlVect* sigma = pnl_vect_create_from_scalar(size,0.2);
    PnlVect* spot = pnl_vect_create_from_scalar(size,100.);
    r=0.1; 
    
    PnlRng *Pnlrng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(Pnlrng, time(NULL));
    RandomGen* rng = new PnlRnd(Pnlrng);

    ParameterEstimation *PrEs = new ParameterEstimation();

    BlackScholesModel *bc = new  BlackScholesModel(size, r , correlation_mat, sigma, spot);
    PnlMat *path = pnl_mat_create(nb_timestep + 1,size);
        
    // 260 is the number of working days per year
    bc->asset(path, T, nb_timestep, rng);
    correlation_estimated =  PrEs->getCorrelationMatrix(path);
    for(int i = 0;i <size;i++){
        for(int j = 0 ;j<i;j++){
            //assertion
            EXPECT_NEAR(pnl_mat_get(correlation_estimated,i,j),pnl_mat_get(correlation_mat,i,j),pnl_mat_get(correlation_mat,i,j)*0.1); 
        }
    }
        
    
    pnl_mat_free(&correlation_estimated);
    pnl_vect_free(&sigma);
    pnl_vect_free(&difference_abs);
    pnl_mat_free(&path);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}