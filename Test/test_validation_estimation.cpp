#include "BlackScholesModel.hpp"
#include "ParameterEstimation.hpp"
#include "stdlib.h"

using namespace std;
int main(){
    int size,nb_timestep;
    double r ,T,rho;
    double vol = 0.05;
    PnlVect *sigma_estimated;
    size = 30; 
    rho = 0;
    T = 8;
    nb_timestep = T * 260;
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

    

    for(int i =0;i<size;i++){
        //pnl_vect_set(sigma,i,vol);
        vol+= 0.05;
    }
    PnlMat *path = pnl_mat_create(nb_timestep + 1,size);
        
    // 260 is the number of working days per year
    bc->asset(path, T, nb_timestep, rng);
    sigma_estimated =  PrEs->getVolatilitiesVector(path);
    cout << "taille de la fenêtre d'estimation :"<<nb_timestep<<" T =  "<<T<<endl;
    cout << "vecteur des volatilités pris pour la simulation des actifs : "<<endl;
    pnl_vect_print(sigma);
    cout << "vecteur des volatilités estimées :  "<<endl;
    pnl_vect_print(sigma_estimated);
    cout << " la moyenne empérique du vecteur des volatilités estimées  : "<< endl;
    double esp =pnl_vect_sum(sigma_estimated)/size; 
    cout <<esp<<endl;
    cout<<" L écart type : "<<endl;
    PnlVect* volati_square = pnl_vect_create(size); 
    pnl_vect_clone(volati_square,sigma_estimated);
    pnl_vect_mult_vect_term(volati_square, sigma_estimated);
    cout<<sqrt(pnl_vect_sum(volati_square)/size-pow(esp,2))<<endl;


        
    
    pnl_vect_free(&sigma_estimated);
    pnl_vect_free(&sigma);
    pnl_vect_free(&difference_abs);
    pnl_mat_free(&path);

    return 0;
}