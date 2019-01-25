#include "HedgePortfolio.hpp"
#include "Option.hpp"
#include "Kozei.hpp"
#include "SimulatedDataProvider.hpp"
#include <ctime>
#include <omp.h>

using namespace std;

int main(int argc,char **argv){

  //Variables
  double r = 0.03;
  double rho = 0.1;
  double sigmaValue = 0.4;
  double spotValue = 100;
  double inv_init = 100;
  size_t n_samples = 50000;
  double fdStep = 0.1;
  int nbRebalancementPerStep = 1;

  // Initialisaton du Kozei
  Kozei *K = new Kozei(inv_init);


  // Initialisation du Random Number Generator + Vect spot + Vect sigma
  PnlRng* pnlRng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(pnlRng, time(NULL));
  RandomGen* rng = new PnlRnd(pnlRng);
  PnlVect *spot = pnl_vect_create_from_scalar(K->size_, spotValue);
  PnlVect *sigma = pnl_vect_create_from_scalar(K->size_, sigmaValue);


  // Initialisation du simulateur de données de marché
  SimulatedDataProvider* Simulator = new SimulatedDataProvider(rng,  spot, r , rho , sigmaValue);

  PnlMat* marketData = Simulator->getMarketData(K->T_, K->nbTimeSteps_, nbRebalancementPerStep);

  // Initialisation du modèle de BlackScholes
  BlackScholesModel *bs_model = new BlackScholesModel(K->size_, r, rho, sigma, spot);

  // Initialisation du modèle de MonteCarlo
  MonteCarlo *mc_pricer = new MonteCarlo(bs_model, K , rng, fdStep, n_samples);

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
