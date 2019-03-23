#include "ForwardTest.hpp"
#include "Asian.hpp"

using namespace std;

double T = 1;
int nbTimeSteps = 13;
int size = 1;
double strike = 10;
PnlVect *weights = pnl_vect_create_from_double(size, 1.0/size);

Asian* asian = new Asian(T,nbTimeSteps,size,strike,weights);

double fdStep = 0.1;
int nbSamples = 50000;
double r = 0.04879;
double rho = 0;
double sigmaValue = 0.2;
double spotValue = 10;
ForwardTest* forwardTest = new ForwardTest(asian,r,rho,sigmaValue,spotValue,nbSamples,fdStep);


int step = T*Tools::NumberOfDaysPerYear/nbTimeSteps;

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
