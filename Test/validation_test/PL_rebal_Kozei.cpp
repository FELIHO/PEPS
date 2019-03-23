#include "ForwardTest.hpp"
#include "Kozei.hpp"
#include <vector>

using namespace std;

Kozei *kozei = new Kozei(10);

double fdStep = 0.1;
int nbSamples = 50000;
double r = 0.04879;
double rho = 0;
double sigmaValue = 0.2;
double spotValue = 10;
ForwardTest* forwardTest = new ForwardTest(kozei, r, rho, sigmaValue, spotValue, nbSamples, fdStep);

int step = kozei->T_*Tools::NumberOfDaysPerYear/kozei->nbTimeSteps_;

vector<int> V = Tools::Divisors(step);

int main(int argc, char const *argv[])
{
    for (int nbRebalancementPerStep : V){
        for(int i =0; i<1000; i++){
            forwardTest->setRebalancementFrequence(nbRebalancementPerStep);
            double PL = forwardTest->ProfitAndLoss();
            cout << PL << " ";
        }
        cout << endl;
        
    }
    return 0;
}
