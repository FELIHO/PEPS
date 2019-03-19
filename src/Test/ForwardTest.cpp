#include "HedgePortfolio.hpp"
#include "Option.hpp"
#include "Kozei.hpp"
#include "SimulatedDataProvider.hpp"
#include <ctime>
#include <omp.h>

#include "ForwardTest.hpp"

using namespace std;


ForwardTest::ForwardTest(Option* opt, RandomGen* rng, PnlVect* spot, double r, double rho, double sigmaValue, double spotValue, int n_samples, double fdStep){

      Simulator_ = new SimulatedDataProvider(rng,  spot, r , rho , sigmaValue);

      PnlVect *spot = pnl_vect_create_from_scalar(opt->size_, spotValue);
      PnlVect *sigma = pnl_vect_create_from_scalar(opt->size_, sigmaValue);
      BlackScholesModel *bs_model = new BlackScholesModel(opt->size_, r, rho, sigma, spot);

      monteCarlo_ = new MonteCarlo(bs_model, opt, fdStep, n_samples);
      
      marketData_ = pnl_mat_new();
}

void ForwardTest::simutaleMarket(int nbRebalancementPerStep) {
    marketData_ = Simulator_->getMarketData(monteCarlo_->opt_->T_, monteCarlo_->opt_->nbTimeSteps_, nbRebalancementPerStep);
}

void ForwardTest::simutaleDailyMarket() {
    marketData_ = Simulator_->getDailyMarketData(monteCarlo_->opt_->T_);
}

void ForwardTest::simutaleWeeklyMarket() {
    marketData_ = Simulator_->getWeeklyMarketData(monteCarlo_->opt_->T_, monteCarlo_->opt_->nbTimeSteps_);
}

void ForwardTest::simutaleMonthlyMarket() {
    marketData_ = Simulator_->getMonthlyMarketData(monteCarlo_->opt_->T_, monteCarlo_->opt_->nbTimeSteps_);
}

void ForwardTest::simutaleMonthlyMarket() {
    marketData_ = Simulator_->getMonthlyMarketData(monteCarlo_->opt_->T_, monteCarlo_->opt_->nbTimeSteps_);
}


int main(int argc,char **argv){


  // Matrix
  PnlMat *path = pnl_mat_create(K->nbTimeSteps_ + 1, K->size_);



  // Initializing Variables to contain the test results
  double prix;
  double ic;

  float time;
  clock_t t0,tf;

  t0 = omp_get_wtime();


  mc_pricer->price(prix,ic);

  // Variables temporaires
  PnlVect *delta = pnl_vect_create_from_scalar(K->size_,0);
  PnlMat* past = pnl_mat_create_from_scalar(1,K->size_ , 100) ;

  mc_pricer->delta(past,0.0,delta);

  tf = omp_get_wtime();

  cout << endl;
  cout << "#####################" << endl;
  cout << "# TEMPS D'EXECUTION #   =   " << tf - t0 << endl;
  cout << "#####################" << endl << endl;
  cout << endl;
  cout << "###############" << endl;
  cout << "# DELTA à t=0 #   =   "<< endl << endl;
  pnl_vect_print(delta);
  cout << "###############" << endl << endl;

  cout << "##########################" << endl;
  cout << "# PRIX DE L'OPTION À t=0 #   =   "<< prix << endl;
  cout << "##########################" << endl << endl;

  cout << "###########################" << endl;
  cout << "# INTERVALLE DE CONFIANCE #   =   "<< ic << endl;
  cout << "###########################" << endl << endl;



  t0 = omp_get_wtime();

  HedgePortfolio hedgePortfolio = HedgePortfolio(marketData, mc_pricer);
  double PL = hedgePortfolio.HedgeError(marketData);

  tf = omp_get_wtime();

  cout << endl;
  cout << "#################" << endl;
  cout << "# PROFIT & LOSS #   =   "<< PL << endl;
  cout << "#################" << endl;

  cout << endl;
  cout << "#####################" << endl;
  cout << "# TEMPS D'EXECUTION #   =   " << tf - t0 << endl;
  cout << "#####################" << endl << endl;





  delete(K);
  delete(rng);
  pnl_rng_free(&pnlRng);
  pnl_vect_free(&spot);
  pnl_vect_free(&sigma);
  delete(Simulator);
  pnl_mat_free(&marketData);
  delete(bs_model);
  delete(mc_pricer);
  pnl_mat_free(&path);
  pnl_vect_free(&delta);
  pnl_mat_free(&past);

  return 0;
}
