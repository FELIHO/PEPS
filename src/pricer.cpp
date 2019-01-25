
//#include "BlackScholesModel.hpp"
//#include "MonteCarlo.hpp"
#include "HedgePortfolio.hpp"
#include "jlparser/parser.hpp"
//#include "Option.hpp"
#include "Basket.hpp"
#include "Performance.hpp"
#include "Asian.hpp"
#include <ctime>

using namespace std;

int main(int argc,char **argv){

  // Argument managment
  const char *argument = argv[1];
  const char *file;
  string c = "-c";
  PnlMat *past;
  const char* char_c = c.c_str();

  if (!strcmp(argument, char_c)){ // if the -c option is given
    file = argv[3]; // data_input to parse
    char *marketData = argv[2]; // market_file to parse
    past = pnl_mat_create_from_file(marketData); // market data matrix
  }
  else
  { // the -c option isn't given
    file = argv[1]; // data_input to parse
  }

  // Variables
  double T, r, strike;
  PnlVect *spot, *sigma, *trend;
  string type;
  int size;
  size_t n_samples;
  int nbTimeSteps;
  double rho, fdStep;
  int H;

  // Initializing Variables to contain the test results
  double sd;
  //double payoff = 0;
  double prix;
  double ic;

  Param *P = new Parser(file);

  P->extract("option type", type);
  P->extract("maturity", T);
  P->extract("option size", size);
  P->extract("timestep number", nbTimeSteps);

  // Matrix
  PnlMat *path = pnl_mat_create(nbTimeSteps + 1, size);
  PnlVect *weights = pnl_vect_create(size);

  P->extract("spot", spot, size);
  P->extract("volatility", sigma, size);
  P->extract("interest rate", r);
  P->extract("sample number", n_samples);
  P->extract("correlation", rho);
  P->extract("payoff coefficients", weights, size);
  P->extract("fd step", fdStep);

  P->extract("hedging dates number", H);
  P->extract("trend", trend, size);

  // Initializing Option
  Option *opt;

  if (type =="basket")
  {
    P->extract("strike", strike);
    opt = new Basket(T,nbTimeSteps,size,strike,weights);
  }
  else if ( type == "asian")
  {
    P->extract("strike", strike);
    opt = new Asian(T,nbTimeSteps,size,strike,weights);
  }
  else if ( type == "performance")
  {
    opt = new Performance(T,nbTimeSteps,size,weights);
  }
  else
  {
    cout << "Invalid option type parsed" << endl;
  }

  // Initializing Random Number Generator
  PnlRng* pnlRng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(pnlRng, time(NULL));
  RandomGen* rng = new PnlRnd(pnlRng);


  BlackScholesModel *bs_model;


  if (!strcmp(argument, char_c)) // option -c is given
  {

    bs_model = new BlackScholesModel(size, r, rho, sigma, spot, trend);

    //bs_model->simul_market(past,T,H,rng);
    //bs_model->asset(path, 0.0, T, nbTimeSteps, rng, past);

    //PnlMat* dataSimul = pnl_mat_new();
    //bs_model->simul_market(dataSimul, T, H, rng);

  }
  else // option -c is not given
  {

    //bs_model->asset(path,T,nbTimeSteps,rng);
    bs_model = new BlackScholesModel(size, r, rho, sigma, spot);

    PnlMat* dataSimul = pnl_mat_new();
    bs_model->asset(dataSimul, T, H, rng);

  }


  MonteCarlo *mc_pricer = new MonteCarlo(bs_model, opt, fdStep, n_samples);

  if (strcmp(argument, char_c)) // option -c not given
  {

    float time;
    clock_t t0,tf;

    t0 =  omp_get_wtime();

    mc_pricer->price(prix,ic);

    // Variables temporaires
    PnlVect *delta = pnl_vect_create_from_scalar(size,0.0);
    past = pnl_mat_create_from_scalar(1,size,pnl_vect_get(spot,0));

    mc_pricer->delta(past,0.0,delta);

    tf =  omp_get_wtime();

    cout << endl;
    cout << "#####################" << endl;
    cout << "# TEMPS D'EXECUTION #   =   " << tf - t0 << endl;
    cout << "#####################" << endl << endl;
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
    pnl_vect_free(&delta);

  }
  else // option -c given
  {

    float time;
    clock_t t0,tf;

    t0 = omp_get_wtime();

    HedgePortfolio hedgePortfolio = HedgePortfolio(past, mc_pricer);
    double PL = hedgePortfolio.HedgeError(past);

    tf = omp_get_wtime();

    cout << "#################" << endl;
    cout << "# PROFIT & LOSS #   =   "<< PL << endl;
    cout << "#################" << endl;

    cout << endl;
    cout << "#####################" << endl;
    cout << "# TEMPS D'EXECUTION #   =   " << tf - t0 << endl;
    cout << "#####################" << endl << endl;
  }


  pnl_mat_free(&past);
  pnl_mat_free(&path);
  pnl_vect_free(&spot);
  pnl_vect_free(&sigma);
  pnl_vect_free(&trend);
  pnl_vect_free(&weights);
  delete P;
  delete(rng);
  delete mc_pricer;
  delete bs_model;
  delete opt;

  return 0;

}
