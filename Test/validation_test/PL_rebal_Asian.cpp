#include "ForwardTest.hpp"
#include "Asian.hpp"

using namespace std;

double T = 1;
int nbTimeSteps = 13;
int size = 1;
double strike = 100;
PnlVect *weights = pnl_vect_create_from_double(size, 1.0/size);

Asian* asian = new Asian(T,nbTimeSteps,size,strike,weights);

double fdStep = 0.1;
int nbSamples = 50000;
double r = 0.04879;
double rho = 0;
double sigmaValue = 0.2;
double spotValue = 100;
ForwardTest* forwardTest = new ForwardTest(asian,r,rho,sigmaValue,spotValue,nbSamples,fdStep);


int step = T*Tools::NumberOfDaysPerYear/nbTimeSteps;

vector<int> V = Tools::Divisors(step);

int main(int argc, char const *argv[])
{
    for (int nbRebalancementPerStep : V){
        cout << nbRebalancementPerStep << " ";
        for(int i =0; i<100; i++){
            forwardTest->setRebalancementFrequence(nbRebalancementPerStep);
            double PL = forwardTest->ProfitAndLoss();
            cout << PL << " ";
        }
        cout << endl;
        
    }
    return 0;
}
