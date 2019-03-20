#include "ForwardTest.hpp"
#include "Basket.hpp"
#include <vector>

using namespace std;

double T = 8;
int nbTimeSteps = 1;
int size = 1;
double strike = 10;
PnlVect *weights = pnl_vect_create_from_double(size, 1.0/size);

Basket* call = new Basket(T, nbTimeSteps, size, strike, weights);

double fdStep = 0.1;
int nbSamples = 50000;
double r = 0.04879;
double rho = 0;
double sigmaValue = 0.2;
double spotValue = 10;
ForwardTest* forwardTest = new ForwardTest(call,r,rho,sigmaValue,spotValue,nbSamples,fdStep);

int step = T*260/nbTimeSteps;

vector<int> V = Tools::Divisors(step);

int main(int argc, char const *argv[])
{
    for (int nbRebalancementPerStep : V){
        forwardTest->setRebalancementFrequence(nbRebalancementPerStep);
        double PL = forwardTest->ProfitAndLoss();
        cout << "nbr de rebalancement :    " << nbRebalancementPerStep*nbTimeSteps << "      PL: "<< PL << endl;
    }
    return 0;
}
