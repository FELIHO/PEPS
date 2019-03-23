#include "ForwardTest.hpp"

using namespace std;


ForwardTest::ForwardTest(Option* opt, double r, double rho, double sigmaValue, double spotValue, int n_samples, double fdStep){

        PnlVect *spot = pnl_vect_create_from_scalar(opt->size_, spotValue);
        PnlVect *sigma = pnl_vect_create_from_scalar(opt->size_, sigmaValue);
        BlackScholesModel *bs_model = new BlackScholesModel(opt->size_, r, rho, sigma, spot);
        monteCarlo_ = new MonteCarlo(bs_model, opt, fdStep, n_samples);

        PnlRng* pnlRng = pnl_rng_create(PNL_RNG_MERSENNE);
        pnl_rng_sseed(pnlRng, time(NULL));
        RandomGen* rng = new PnlRnd(pnlRng);
        SimulatedDataProvider* Simulator = new SimulatedDataProvider(rng, r , rho , sigma, spot);
        marketData_ = Simulator->getDailyMarketData(opt->T_);

        DS_ = new DataSelecter(marketData_);

        pnl_vect_free(&spot);
        pnl_vect_free(&sigma);
        pnl_rng_free(&pnlRng);
        delete(rng);
        delete(Simulator);
        delete(bs_model);
}

ForwardTest::ForwardTest(Option* opt, double r, double rho, PnlVect *sigma, PnlVect *spot, int n_samples, double fdStep){

        BlackScholesModel *bs_model = new BlackScholesModel(opt->size_, r, rho, sigma, spot);
        monteCarlo_ = new MonteCarlo(bs_model, opt, fdStep, n_samples);

        PnlRng* pnlRng = pnl_rng_create(PNL_RNG_MERSENNE);
        pnl_rng_sseed(pnlRng, time(NULL));
        RandomGen* rng = new PnlRnd(pnlRng);
        SimulatedDataProvider* Simulator = new SimulatedDataProvider(rng, r , rho , sigma, spot);
        marketData_ = Simulator->getDailyMarketData(opt->T_);

        DS_ = new DataSelecter(marketData_);

        pnl_rng_free(&pnlRng);
        delete(rng);
        delete(Simulator);
        delete(bs_model);
}

ForwardTest::~ForwardTest() {
  delete(DS_); 
  delete(monteCarlo_);
  pnl_mat_free(&marketData_);
}

void ForwardTest::setRebalancementFrequence(int nbRebalancementPerStep) {
    marketData_ = DS_->getData(monteCarlo_->opt_->T_,monteCarlo_->opt_->nbTimeSteps_, nbRebalancementPerStep); 
}

double ForwardTest::price(double t){
    double prix,ic;
    if (t==0){
        monteCarlo_->price(prix,ic);
    }
    else{
        PnlMat* past = DS_->getPast(t, monteCarlo_->opt_->T_, monteCarlo_->opt_->nbTimeSteps_);
        monteCarlo_->price(past,t,prix,ic);
        pnl_mat_free(&past);
    }
    return prix;
}

PnlVect* ForwardTest::delta(double t){
    PnlVect *delta = pnl_vect_create_from_scalar(monteCarlo_->opt_->size_,0);
    PnlMat* past = DS_->getPast(t, monteCarlo_->opt_->T_, monteCarlo_->opt_->nbTimeSteps_);
    monteCarlo_->delta(past,t,delta);
    pnl_mat_free(&past);
    return delta;
}

double ForwardTest::ProfitAndLoss(){
    HedgePortfolio* hedgePortfolio =new HedgePortfolio(marketData_, monteCarlo_);
    double PL = hedgePortfolio->HedgeError(marketData_);
    delete(hedgePortfolio);
    return PL;
}
