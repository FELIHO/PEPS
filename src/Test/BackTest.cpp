#include "BackTest.hpp"

using namespace std;


BackTest::BackTest(Option* opt, double r, char const *fileData, int firstDateIndex, int n_samples, double fdStep){

        if(firstDateIndex<30){
            throw invalid_argument("les données sont insuffisantes pour estimer les paramètres: minimum 30 jours");
        }

        PnlMat* dailyData = pnl_mat_create_from_file(fileData);

        if(dailyData->m-firstDateIndex < opt->T_*Tools::NumberOfDaysPerYear){
            throw length_error("les données sont insuffisantes pour effectuer un backTest");
        }
        
        DS_ = new DataSelecter(dailyData,firstDateIndex);

        marketData_ = DS_->getData(opt->T_,opt->T_*Tools::NumberOfDaysPerYear);

        PnlMat* historicalEstimationWindow = DS_->getHistoricalEstimationWindow();
        ParameterEstimation pe ;
        PnlVect* sigma = pe.getVolatilitiesVector(historicalEstimationWindow);
        PnlMat* rho = pe.getCorrelationMatrix(historicalEstimationWindow);

        PnlVect *spot= pnl_vect_new();
        pnl_mat_get_row(spot,marketData_,0);

        BlackScholesModel *bs_model = new BlackScholesModel(opt->size_, r, rho, sigma, spot);
        monteCarlo_ = new MonteCarlo(bs_model, opt, fdStep, n_samples);

        pnl_vect_free(&spot);
        pnl_vect_free(&sigma);
        pnl_mat_free(&rho);
        pnl_mat_free(&dailyData);
        delete(bs_model);
}

void BackTest::setRebalancementFrequence(int nbRebalancementPerStep) {
    marketData_ = DS_->getData(monteCarlo_->opt_->T_,monteCarlo_->opt_->nbTimeSteps_, nbRebalancementPerStep); 
}

double BackTest::price(double t){
    double prix,ic;
    if (t==0){
        monteCarlo_->price(prix,ic);
    }
    else{
        PnlMat* past = DS_->getPast(marketData_, monteCarlo_->opt_->T_, t , monteCarlo_->opt_->nbTimeSteps_);
        monteCarlo_->price(past,t,prix,ic);
        pnl_mat_free(&past);
    }
    return prix;
}

PnlVect* BackTest::delta(double t){
    PnlVect *delta = pnl_vect_create_from_scalar(monteCarlo_->opt_->size_,0);
    PnlMat* past = DS_->getPast(marketData_, monteCarlo_->opt_->T_, t , monteCarlo_->opt_->nbTimeSteps_);
    monteCarlo_->delta(past,t,delta);
    pnl_mat_free(&past);
    return delta;
}

double BackTest::ProfitAndLoss(){
    HedgePortfolio* hedgePortfolio =new HedgePortfolio(marketData_, monteCarlo_);
    double PL = hedgePortfolio->HedgeError(marketData_);
    delete(hedgePortfolio);
    return PL;
}
