#include "BlackScholesModel.hpp"
#include "ParameterEstimation.hpp"
#include "DataSelecter.hpp"
#include <assert.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

using namespace std;
using namespace Computations;
int main(){
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        } else {
        perror("getcwd() error");
        return 1;
    }

    string way;
    way.assign(cwd);
    size_t pos = way.find("/PEPS/");
    string str3 = way.substr(0,pos+6);
    string str4 = way.substr(0,pos+6);
    string str5 = way.substr(0,pos+6);


    str3 = str3+"DataFeeds/kozei_dataExchangeRates.dat";
    const char *cstr = str3.c_str();
    PnlMat *allExchangeData = pnl_mat_create_from_file(cstr);

    str4 = str4+"DataFeeds/kozei_IndexdataExchangeRates.dat";
    const char *cstr2 = str4.c_str();
    PnlVectInt *dateIndexes = pnl_vect_int_create_from_file(cstr2);

    str5 = str5+"DataFeeds/kozei_Constatation.dat";
    const char *cstr3 = str5.c_str();
    PnlVectInt *constationDate = pnl_vect_int_create_from_file(cstr3);

    DataSelecter ds = DataSelecter(constationDate);

    int date;
    cout << "Entrez une date comprise entre 01/01/1998(19980101) et la 07/01/2019(20190107) par défaut si vous prenez 20180515 le test donne une matrice de taille 10(spot + nbDate de constation): " << endl;
    cin >> date;
    
    PnlMat *past = ds.getPast(allExchangeData, dateIndexes, date);
    cout << "voici votre matrice past" << endl;
    pnl_mat_print(past);
    cout<< past->m << endl;

    int timeEstimation;
    cout << "Entrez une durée fenêtre d'estimation un entier, en nombre de jour: " << endl;
    cin >> timeEstimation;


    int size = 5;
    double r ;

    cout << "Entrez un taux d'intérêt EUR(taux * 100): " << endl;
    cin >> r;
    r/=100;
    
    PnlVect *interestRates = pnl_vect_create_from_double(5,0.1);
    
    double interest_rate_USD; 
    double interest_rate_JAP;
    double interest_rate_GBP;
    double interest_rate_CHF;
    double interest_rate_BRL;

    cout << "Entrez un taux d'intérêt USD(taux * 100): " << endl;
    cin >> interest_rate_USD;
    
    cout << "Entrez un taux d'intérêt JAP(taux * 100): " << endl;
    cin >> interest_rate_JAP;
    
    cout << "Entrez un taux d'intérêt GBP(taux * 100): " << endl;
    cin >> interest_rate_GBP;
    
    cout << "Entrez un taux d'intérêt CHF(taux * 100): " << endl;
    cin >> interest_rate_CHF;
    
    cout << "Entrez un taux d'intérêt BRL(taux * 100): " << endl;
    cin >> interest_rate_BRL;

    pnl_vect_set(interestRates,0,interest_rate_USD/100);
    pnl_vect_set(interestRates,1,interest_rate_JAP/100);
    pnl_vect_set(interestRates,2,interest_rate_GBP/100);
    pnl_vect_set(interestRates,3,interest_rate_CHF/100);
    pnl_vect_set(interestRates,4,interest_rate_BRL/100);
    
    PnlVect* spot = pnl_vect_new();
    pnl_mat_get_row(spot, past, 0);

    //Parametres estimation
    ParameterEstimation pe;
    PnlMat window = ds.getWindowPreviousEstimation(allExchangeData, dateIndexes, date, timeEstimation);
    PnlMat *correlationmatrix = pe.getCorrelationMatrix(&window); 
    PnlVect *volatiliesmatrix = pe.getVolatilitiesVector(&window);

    BlackScholesModel *bc = new BlackScholesModel(size,r,interestRates,correlationmatrix,volatiliesmatrix,spot);
    
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    //17 correspond aux dates de constations + la date initiale  
    

    double T = 8.0 + 11.0/365;
    double t = ds.computeTvalue(dateIndexes, pnl_vect_int_get(constationDate, 0), date, T);
    int nbTimeSteps = 16;
    
    PnlMat *path = pnl_mat_create(nbTimeSteps + 1 ,size);
    bc->asset(path,t,T,nbTimeSteps,rng,past);
    
    cout<<" La matrice Path simulée :"<<endl;
    pnl_mat_print(path);
    


}