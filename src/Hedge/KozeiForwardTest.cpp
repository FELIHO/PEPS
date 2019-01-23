#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "HedgePortfolio.hpp"
#include "Kozei.hpp"
#include "DataProvider.hpp"
#include "SimulatedDataProvider.hpp"
#include "jlparser/parser.hpp" 
#include <ctime>
#include <string.h>

using namespace std; 
using namespace Computations; 


// KozeiForwardTest::KozeiForwardTest(){
//   currentRebalancingIndex_ = 0;
//   monteCarlo_ = new MonteCarlo();
//   prix_ = 0;
//   ic_ = 0;
//   past_ = pnl_mat_new();
//   delta_ = pnl_vect_new();
//   S_current = pnl_vect_new();
//   investTauxSansRisque_ = 0;
// }

int main(int argc,char **argv){
    PnlMat* marketData;
    int estimationWindow;

    SimulatedDataProvider* Simulateur = new SimulatedDataProvider();

    Simulateur->GetDataFeed(marketData, 2021, 30);
    
    Kozei* K = new Kozei(100);

    bs_model = new BlackScholesModel(size, r , rho, sigma, spot);
    
    MonteCarlo *mc_pricer = new MonteCarlo(bs_model, opt, rng, fdStep, n_samples);
    
    HedgePortfolio hedgePortfolio = HedgePortfolio(past, mc_pricer);
    
}