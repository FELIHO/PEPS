#include "ForwardTest.hpp"
#include "Kozei.hpp"
#include <vector>

using namespace std;

PnlVect* niveauxInitiaux =pnl_vect_create_from_double(30,100);
Kozei *kozei = new Kozei(100,niveauxInitiaux);

double fdStep = 0.1;
int nbSamples = 50000;
double r = 0.04879;
double rho = 0;
double sigmaValue = 0.2;
double spotValue = 100;
ForwardTest* forwardTest = new ForwardTest(kozei,r,rho,sigmaValue,spotValue,nbSamples,fdStep);

int step = kozei->T_*Tools::NumberOfDaysPerYear/kozei->nbTimeSteps_;

vector<int> V = Tools::Divisors(step);

int main(int argc, char const *argv[])
{
    for (int nbRebalancementPerStep : V){
        if (nbRebalancementPerStep < 130){
            cout << nbRebalancementPerStep << " ";
            for(int i =0; i<20; i++){
                forwardTest->setRebalancementFrequence(nbRebalancementPerStep);
                double PL = forwardTest->ProfitAndLoss();
                cout << PL << " ";
            }
            cout << endl;
        }
    }
    return 0;
}
