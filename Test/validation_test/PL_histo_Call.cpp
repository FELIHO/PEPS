#include "ForwardTest.hpp"
#include "Basket.hpp"
#include <vector>

using namespace std;

double T = 1;
int nbTimeSteps = 1;
int size = 1;
double strike = 100;
PnlVect *weights = pnl_vect_create_from_double(size, 1.0/size);

Basket* call = new Basket(T, nbTimeSteps, size, strike, weights);

double fdStep = 0.1;
int nbSamples = 50000;
double r = 0.04879;
double rho = 0;
double sigmaValue = 0.2;
double spotValue = 100;
ForwardTest* forwardTest = new ForwardTest(call,r,rho,sigmaValue,spotValue,nbSamples,fdStep);


int main(int argc, char const *argv[])
{
    for(int i =0; i<1; i++){
        double PL = forwardTest->ProfitAndLoss();
        cout << PL << endl;
    }
    return 0;
}
